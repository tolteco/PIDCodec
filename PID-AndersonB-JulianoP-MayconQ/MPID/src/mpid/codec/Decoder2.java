/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.codec;

import java.io.File;

/**
 *
 * @author Juliano_Felipe
 */
public class Decoder2 extends Decoder1{
    /* Just a place holder */
    public Decoder2(File video, int FPS) {
        super(video, FPS);
    }

    public Decoder2(File video, short nextFrames, short prevFrames, int framesPerSecond) {
        super(video, nextFrames, prevFrames, framesPerSecond);
    }
    /* Just a place holder */
}
