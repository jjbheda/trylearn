package com.efurture.file;

import com.efurture.file.compress.GZip;
import com.efurture.file.io.Block;
import com.efurture.file.io.BlockFileInputStream;
import com.efurture.file.io.BlockOutputStream;
import com.efurture.file.io.FlushCallback;
import com.efurture.file.meta.Meta;
import com.efurture.file.io.MetaOutputStream;
import com.efurture.file.meta.MetaUtils;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.*;

/**
 * 简单高效的小文件存储的数据库,可以存储海量小文件, 单个文件不超过1G。
 * 多个小文件合并到一个文件中, 基于Append的设计模式, 写入不影响读, 并发性能好。
 * Created by 剑白(jianbai.gbj) on 2017/6/23.
 */
public class FileStore {

    /**
     * Store文件后缀名字
     * */
    public static final String NODE_SUFFIX = ".node";

    /**
     * Meta文件后缀名字
     * */
    public static final String META_SUFFIX = ".meta";

    /**
     * 单个Store的大小,默认1024MB
     * */
    public static final long DEFAULT_STORE_FILE_SIZE = 1024*1024*1024l;

    /**
     * 默认文件的大小
     * */
    private long storeFileSize = DEFAULT_STORE_FILE_SIZE;


    /**
     * 默认压缩的文件后缀名字
     * */
    private List<String> zipFileSuffix = new ArrayList<String>();
    {
        zipFileSuffix.add(".dat");
        zipFileSuffix.add(".txt");
        zipFileSuffix.add(".json");
        zipFileSuffix.add(".html");
        zipFileSuffix.add(".htm");
    }


    /**
     * 文件块写操作
     * */
    private BlockOutputStream outputStream;

    /**
     * 文件的原信息写入
     * */
    private MetaOutputStream metaOutputStream;

    /**
     * 存存储的文件
     * */
    private File dir;

    /**
     * 当前节点
     * */
    private int node;

    /**
     * 文件元信息,包含文件的位置信息
     * */
    private HashMap<String, Meta> fileMeta = new HashMap<String, Meta>();


    /**
     * 最近写入的缓存
     * */
    private LinkedHashMap<String, byte[]> nodeMemoryCache = new LinkedHashMap<String, byte[]>();

    /**
     * 是否同步写入
     * */
    private boolean  writeSyn = true;


    /**
     * 创建一个文件的存储
     * */
    public FileStore(String dir) throws IOException{
        this.dir = new File(dir);
        File file = this.dir;
        if(!file.exists()){
            file.mkdirs();
        }
        if(!file.isDirectory()){
            throw  new IllegalArgumentException("FileStore'path must be dir");
        }
        String[] stores = file.list();
        node = 0;
        if(stores != null){
            List<String> metas  = new ArrayList<String>(4);
            for(String store : stores){
                if(store.endsWith(META_SUFFIX)){
                    metas.add(store);
                }
            }
            Collections.sort(metas, new Comparator<String>() {
                @Override
                public int compare(String o1, String o2) {
                    int  num1 = Integer.parseInt(o1.split("\\.")[0]);
                    int  num2 = Integer.parseInt(o2.split("\\.")[0]);
                    return num1 - num2;
                }
            });
            for(String meta : metas){
                fileMeta.putAll(MetaUtils.readNextMeta(dir + File.separator + meta));
            }
            if(metas.size() > 0) {
                String meta = metas.get(metas.size() - 1);
                node = Integer.parseInt(meta.split("\\.")[0]);
            }
        }
        newStore(node);
    }


    /**
     * 存储文件到数据库中
     * */
    public void put(String fileName, byte[] bts) throws IOException {
        boolean zip = false;
        for(String suffix : zipFileSuffix){
            if(fileName.endsWith(suffix)){
                zip = true;
                break;
            }
        }
        put(fileName, bts, zip);
    }

    /**
     * @param  fileName 文明名字
     * @param  bts  字节个数
     * 存储文件到数据库中, 是否压缩
     * */
    public void put(String fileName, byte[] bts, boolean zip) throws IOException {
        byte header = Meta.DEFAULT_HEADER;
        if(zip){
            bts =  GZip.compress(bts, 0, bts.length);
            header = Meta.GZIP_HEADER;
        }
        putData(fileName, bts, header);
    }

    /**
     * 保存文件的信息, header代表数据的标示
     * */
    private void putData(String fileName, byte[] bts, byte header) throws IOException {
        synchronized (this) {
            List<Block> blocks = outputStream.write(fileName, bts, 0, bts.length);
            nodeMemoryCache.remove(fileName);
            nodeMemoryCache.put(fileName, bts);
            Meta meta = Meta.createMeta(header, fileName, node,  blocks);
            fileMeta.put(fileName, meta);
            metaOutputStream.writeMeta(meta);
            if(writeSyn) {
                outputStream.flush();
            }
            if(outputStream.getSize() > storeFileSize){
                node++;
                newStore(node);
            }
        }
    }

    /**
     * 存储字符串内容,默认压缩存储
     * */
    public void put(String fileName, String content) throws IOException {
         put(fileName, content.getBytes(), true);
    }

    /**
     * 存储字符串内容,是否压缩存储
     * */
    public void put(String fileName, String content, boolean zip) throws IOException {
        put(fileName, content.getBytes(), zip);
    }

    /**
     * 删除文件内容
     * */
    public void remove(String fileName) throws IOException {
        Meta meta = fileMeta.get(fileName);
        if(meta == null){
            return;
        }
        synchronized (this){
            meta.flag = Meta.FLAG_DELETE;
            fileMeta.remove(fileName);
            nodeMemoryCache.remove(fileName);
            metaOutputStream.writeMeta(meta);
        }
    }
    /**
     * 读取文件,若文件不存在,则返回空, 若文件存在,返回文件的内容
     * */
    public byte[] get(String fileName) throws IOException {
        Meta meta = fileMeta.get(fileName);
        if(meta == null){
            return  null;
        }
        byte[] bts = getBlocks(fileName, meta);
        if(meta.header == Meta.DEFAULT_HEADER){
            return  bts;
        }
        if(meta.header == Meta.GZIP_HEADER){
            return  GZip.uncompress(bts, 0, bts.length);
        }
        throw  new RuntimeException("Meta Version Not Supported " + meta.header);
    }


    /**
     * 根据meta元数据,返回block中的原始数据信息, 不进行解压处理
     * */
    private byte[] getBlocks(String fileName, Meta meta) throws IOException {
        byte[] bts = nodeMemoryCache.get(fileName);
        if(bts != null){
            return  bts;
        }
        String nodeFile = dir + File.separator + meta.node + NODE_SUFFIX;
        BlockFileInputStream inputStream = new BlockFileInputStream(nodeFile , meta.blocks);
        ByteArrayOutputStream data = new ByteArrayOutputStream(1024*8);
        byte[] buffer = new byte[1024*8];
        int read = 0;
        while ((read = inputStream.read(buffer, 0)) > 0){
            data.write(buffer, 0, read);
        }
        inputStream.close();
        return  data.toByteArray();
    }

    /**
     * 获取文件的内容,并转换成字符串
     * */
    public String getString(String fileName) throws IOException {
        byte[] bts = get(fileName);
        if(bts == null){
            return  null;
        }
        return  new String(bts);

    }


    /**
     * 把数据flush到磁盘
     * */
    public void flush() throws IOException {
        synchronized (this) {
            if(outputStream != null) {
                outputStream.flush();
            }
            if(metaOutputStream != null) {
                metaOutputStream.flush();
            }
        }
    }

    /**
     * 关闭数据库的操作
     * */
    public void close() throws IOException {
        synchronized (this) {
            if(outputStream != null) {
                outputStream.close();
                outputStream = null;
            }
            if(metaOutputStream != null) {
                metaOutputStream.close();
                metaOutputStream = null;
            }
            nodeMemoryCache.clear();
        }
    }

    /**
     * 压缩当前的存储器, 读取当前节点, 然后把数据写入新的节点,来完成压缩。
     * 若数据
     * */
    public void pack() throws IOException {
        int packEnd = node;
        HashMap<String, Meta> packMap;
        List<String> sortMeta = null;
        synchronized (this){
            sortMeta = MetaUtils.listSortMeta(dir);
            node++;
            newStore(node);
            packMap = (HashMap<String, Meta>) fileMeta.clone();
        }
        Set<Map.Entry<String,Meta>>  entrySet = packMap.entrySet();
        for(Map.Entry<String,Meta> entry : entrySet){
            Meta meta = entry.getValue();
            if(meta.flag == Meta.FLAG_DELETE){
                continue;
            }
            byte[] bts = getBlocks(entry.getKey(), entry.getValue());
            if(bts == null){
                System.err.println("warning key data lost " + entry.getKey());
                continue;
            }
            putData(entry.getKey(), bts, meta.header);
        }

        for(String meta : sortMeta){
            File file = new File(dir + File.separator + meta);
            if(file.exists()){
                file.delete();
            }else{
                System.err.println("warning meta file  lost " + file.getAbsolutePath());
            }
            String node = meta.substring(0, meta.length() - META_SUFFIX.length()) + NODE_SUFFIX;
            File nodeFile = new File(dir + File.separator + node);
            if(nodeFile.exists()){
                nodeFile.delete();
            }else{
                System.err.println("warning node data lost " + nodeFile.getAbsolutePath());
            }
        }
    }

    /**
     * 创建新的存储器
     * */
    private void newStore(int node) throws IOException {
        close();
        String nodeFile = dir + File.separator + node + NODE_SUFFIX;
        String metaFile = dir + File.separator + node + META_SUFFIX;
        outputStream = new BlockOutputStream(nodeFile);
        outputStream.setFlushCallback(new FlushCallback() {
            @Override
            public void onDisk(boolean close) throws IOException {
                if(metaOutputStream != null) {
                    metaOutputStream.flush();
                }
                nodeMemoryCache.clear();
            }
        });
        metaOutputStream = new MetaOutputStream(metaFile);
    }

    /**
     * 返回有效文件的元信息, key为文件名字,  Meta为文件的元信息
     * */
    public Map<String, Meta> getFileMeta(){
        return fileMeta;
    }



    /**
     * 设置是否同步写入
     * */
    public void setWriteSyn(boolean writeSyn) {
        synchronized (this) {
            this.writeSyn = writeSyn;
        }
    }

    /**
     * 获取MemoryCache
     * */
    protected Map<String, byte[]> getMemoryCache(){
        return  nodeMemoryCache;
    }

    /**
     * 是否同步写入
     * */
    public boolean isWriteSyn() {
        return writeSyn;
    }

    /**
     * 单个store的大小
     * */
    public long getStoreFileSize() {
        return storeFileSize;
    }

    /**
     * 默认压缩的文件后缀
     * */
    public List<String> getZipFileSuffix() {
        return zipFileSuffix;
    }

    /**
     * 单个store的大小
     * */
    public void setStoreFileSize(long storeFileSize) {
        this.storeFileSize = storeFileSize;
    }

    /**
     * 默认压缩的文件后缀
     * */
    public void setZipFileSuffix(List<String> zipFileSuffix) {
        this.zipFileSuffix = zipFileSuffix;
    }

    /**
     * 回收流信息
     * */
    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }
}
