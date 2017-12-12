/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.codec;

import java.util.ArrayList;
import java.util.List;
import javafx.scene.paint.Color;

/**
 *
 * @author JFPS
 */
public final class Palette {
    //private final Color[] paleta;
    private final List<Color> paleta;

    public Palette() {
        //paleta = new Color[256];
        paleta = new ArrayList();
        generateColors();
    }
    
    private void generateColors(){
        int i, k; //br
        double j; //g
        int h = 0; //contador
        for (i = 0; i < 256; i+=51) {
            for (j = 0.0; j < 256.0; j+=42.5) {
                for (k = 0; k < 256; k+=51) {
                    //paleta[h] = new Color(k, Math.round(j), i, 1);
                    //paleta[h] = Color.rgb(k, (int) Math.round(j), i);
                    paleta.add(Color.rgb(k, (int) Math.round(j), i));
                    h++;
                }
            }
        }
        //System.out.println(paleta.get(0));
    }
    
    public Color getColor(int index){
        //System.out.println("Get: " + index);
        //index += 126;
        //return paleta[index];
        //System.out.println("Size: " + paleta.size());
        return paleta.get(index);
    }
}
