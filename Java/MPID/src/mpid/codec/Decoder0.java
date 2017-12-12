/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.codec;

import java.io.File;
import javafx.scene.image.Image;

/**
 *
 * @author Juliano_Felipe
 */
public class Decoder0 extends AbsDecoder{

    public Decoder0(File video, int FPS) {
        super(video, FPS);
    }

    public Decoder0(File video, short nextFrames, short prevFrames, int framesPerSecond) {
        super(video, nextFrames, prevFrames, framesPerSecond);
    }

    @Override
    public Image decodeFrame(long frameNumber) {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }

    @Override
    public Image getNextFrame() {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }    

    @Override
    public boolean hasFrames() {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }

    @Override
    public int getMilis() {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }

    @Override
    public void reset() {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }

    @Override
    public Image getCurrentFrame() {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }

    @Override
    public Image getPreviousFrame() {
        throw new UnsupportedOperationException("Erased. Will not use it.");
    }
}
