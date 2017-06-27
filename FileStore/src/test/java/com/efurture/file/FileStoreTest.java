package com.efurture.file;

import junit.framework.TestCase;
import org.junit.Assert;

import java.io.IOException;

/**
 * Created by 剑白(jianbai.gbj) on 2017/6/27.
 */
public class FileStoreTest extends TestCase {


    public void  testWriteSync() throws IOException {
        FileStore db = new FileStore("store");
        db.setWriteSyn(true);
        String key = "putKey";
        String value = "value";
        db.put(key, value);
        Assert.assertEquals(value, db.getString(key));

        db.remove(key);
        Assert.assertEquals(null, db.getString(key));
        db.close();
    }

    public void  testWriteSyncFalse() throws IOException {
        FileStore db = new FileStore("store");
        db.setWriteSyn(false);
        String key = "putKeyAsync";
        String value = "value3";
        db.put(key, value);
        Assert.assertEquals(value, db.getString(key));
        db.remove(key);
        Assert.assertEquals(null, db.getString(key));
        db.close();
    }


    public void  testMemoryCache() throws IOException {
        FileStore db = new FileStore("store");
        db.setWriteSyn(true);
        String keyPrefix = "memeoyCache";
        String value = "Repeatedly creating DataOutputStream and DataInputStream instances is not good for performance.\n" +
                "\n" +
                "However, I suspect that a more important performance issue is that you are reading and writing without any Java-side buffering. This means that each read / write call is making one (and possibly many) syscalls to read data. System calls are a couple of orders of magnitude slower than ordinary Java method calls.\n" +
                "\n" +
                "You need to wrap the Socket input stream in a BufferedInputStream and the output stream in a BufferedOutputStream, and then wrap these in the Data streams. (Note that when you do this, it becomes more important that you flush the DataOutputStream at the end of each message, or series of messages.\n" +
                "\n" +
                "Do I write a few info, and then flush after every each write?\n" +
                "Like I said above, you flush at the end of each message, or each sequence of messages. Each flush on your buffered output stream will result in a syscall, assuming there is data in the buffer to be flushed. If you flush on every write you are (probably) doing unnecessary syscalls.\n" +
                "\n" +
                "It is up to you to figure out where your protocol's message boundaries ought to be. It depends entirely on the details of the data you are sending / receiving, and the way it is processed.";
        for(int i=0; i<100000; i++) {
            db.put(keyPrefix + i, value, false) ;
        }
        Assert.assertEquals(0, db.getMemoryCache().size());

        db.setWriteSyn(false);

        for(int i=0; i<100000; i++) {
            db.put(keyPrefix + i, value, false) ;
        }
        Assert.assertTrue(db.getMemoryCache().size() > 0);


        db.close();
    }


    public void  testPack() throws IOException {
        FileStore db = new FileStore("store");
        db.setWriteSyn(false);
        db.pack();
        db.close();
    }
}