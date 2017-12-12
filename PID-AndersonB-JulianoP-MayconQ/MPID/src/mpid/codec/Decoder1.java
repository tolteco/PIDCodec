/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.codec;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.scene.image.Image;
import javafx.scene.image.WritableImage;

/**
 *
 * @author Juliano_Felipe
 */
public class Decoder1 extends AbsDecoder{
    protected final Palette paleta = new Palette();
    protected byte[] buffer;
    private int frameCount = 0;
    
    public Decoder1(File video, int FPS) {
        super(video, FPS);
    }

    public Decoder1(File video, short nextFrames, short prevFrames, int framesPerSecond) {
        super(video, nextFrames, prevFrames, framesPerSecond);
    }

    @Override
    public Image decodeFrame(long frameNumber) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    private void bootstrap(){
        byte[] header = new byte[4];
        
        try {
            in = new FileInputStream(video);
            in.read(header);
            //System.out.println("Read into header");
           // System.out.println("Header[0]" + header[0]);
            
            //if (header[0] != 48) throw new IllegalStateException("Versão não suportada por Decoder1"); //No formato escrito, 48 = 2;
            //int inn = ((header[0] << 8)) | (header[1] & 0x000000ff);
            //int heigh = ((header[0] << 8) & 0x0000ff00) | (header[1]);
            //Números sem muito sentido estão impressos no arquivo. Vou dar "hardcode"
            
            height = 1080;
            width  = 1920;
                   
            buffer = new byte[width];
            frameCount = 0;
        } catch(IOException | IllegalStateException e) {
            e.printStackTrace();
        }
    }
    
    private void readCurrent(){
        int lineCount = 0;
        WritableImage img = new WritableImage(width, height);
        while (lineCount < height){
            try {
                in.read(buffer);
            } catch (IOException ex) {
                Logger.getLogger(Decoder1.class.getName()).log(Level.SEVERE, null, ex);
            }
            int colCount = 0;
            while (colCount < width){
                int ind = buffer[colCount] & 0xff;
                img.getPixelWriter().setColor(colCount, height-lineCount-1, paleta.getColor(ind));
                ++colCount;
            }
            ++lineCount;
        }
        frameCount++;
        current = img;
    }
    
    @Override
    public Image getNextFrame() {
        if (current == null){
            bootstrap();
        }
        
        readCurrent();
        return current;
    }

    @Override
    public boolean hasFrames() {
        if (current == null) return true;
        
        try {
            //System.out.println("AVIL: " + in.available());
            return in.available() >= width;
        } catch (IOException ex) {
            Logger.getLogger(Decoder1.class.getName()).log(Level.SEVERE, null, ex);
        }
        return false;
    }

    @Override
    public int getMilis() {
        return milisDelay;
    }

    @Override
    public void reset() {
        bootstrap();
    }

    @Override
    public Image getCurrentFrame() {
        return current;
    }

    @Override
    public Image getPreviousFrame(){
        if (current == null){ return null;}
        if (frameCount == 0) return current;
        if (frameCount == 1) return current;
        
        previousFrameOffset();
        return getNextFrame();
    }
    
    private void previousFrameOffset(){
        try {
            in = new FileInputStream(video);
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Decoder1.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        long calc = 4 + ((frameCount-2)*width*height);
        try {
            in.skip(calc);
        } catch (IOException ex) {
            Logger.getLogger(Decoder1.class.getName()).log(Level.SEVERE, null, ex);
        }
        frameCount--;
        frameCount--;
    }
}
