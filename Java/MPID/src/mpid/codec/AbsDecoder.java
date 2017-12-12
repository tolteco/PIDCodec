/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.codec;

import java.io.File;
import java.io.FileInputStream;
import javafx.scene.image.Image;

/**
 *
 * @author JFPS
 */
public abstract class AbsDecoder implements Decoder{
    public static final short NEXT_FRAMES = 0;
    public static final short PREV_FRAMES = 0;
    
    protected final short nextFrames;
    protected final short prevFrames;
    protected final File video;
    protected final int milisDelay;

    protected Image[] previous;
    protected Image current;
    protected Image[] next;
    
    protected int width;
    protected int height;
    
    protected FileInputStream in;
    
    public AbsDecoder(File video, int FPS) {
        this(video, PREV_FRAMES, NEXT_FRAMES, FPS);
    }

    public AbsDecoder(File video, short nextFrames, short prevFrames, int framesPerSecond) {
        this.nextFrames = nextFrames;
        this.prevFrames = prevFrames;
        this.video = video;
        this.milisDelay = (int) ((1.0/framesPerSecond)*1000); //Delay in miliseconds
        
        previous = new Image[prevFrames];
        next = new Image[nextFrames];
        current = null;
    }    
    //https://stackoverflow.com/questions/20676858/javafx-convert-writableimage-to-image
    
}
