/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.codec;

import javafx.scene.image.Image;

/**
 *
 * @author Juliano_Felipe
 */
public interface Decoder {
    public Image decodeFrame(long frameNumber);
    public Image getNextFrame();
    public boolean hasFrames();
    public int getMilis();
    public void reset();
    public Image getCurrentFrame();
    public Image getPreviousFrame();
}
