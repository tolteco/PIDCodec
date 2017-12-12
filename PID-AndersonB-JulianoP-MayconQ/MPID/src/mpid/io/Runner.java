/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.io;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import static java.nio.file.StandardCopyOption.REPLACE_EXISTING;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import mpid.error.ErrorCode;
import mpid.error.Error;

/**
 *
 * @author JFPS
 */
public class Runner {
    public static final File CODEC = new File("Codif.exe");
    private final List<File> arquivos;
    private final File folder;
    private final String outName;

    public Runner(List<File> arquivos, File folder, String outName) {
        this.arquivos = arquivos;
        this.folder = folder;
        this.outName = outName;
    }
    
    public ErrorCode run(){
        Error err = new Error();
        StringBuilder hugeBuild = new StringBuilder();
        arquivos.forEach((f) -> {
            hugeBuild.append(f.getAbsolutePath()).append(" ");
        });
        
        hugeBuild.deleteCharAt(hugeBuild.length()-1);
        String hugeString = hugeBuild.toString();
        
        Process p;
        try {
            /*System.out.println("TRY: " + CODEC.getName() + " " +
                    outName + " " + hugeString);*/
            p = Runtime.getRuntime().exec(CODEC.getName() + " " +
                    outName + " " + hugeString);
            
            try {
                int errorCode = p.waitFor();
                ErrorCode errr = err.getError(errorCode);
                if (errr == ErrorCode.Err_0){
                    try {
                        File outFile = new File(folder.getAbsolutePath()+"\\"+outName);
                        File oldOut = new File(outName);
                        //System.out.println(outFile.getAbsolutePath());
                        if (!outFile.exists()) outFile.createNewFile();
                        Files.copy(oldOut.toPath(), outFile.toPath(), REPLACE_EXISTING);
                        oldOut.delete();
                    } catch (IOException ex) {
                        Logger.getLogger(Runner.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
                return errr;
                
            } catch (InterruptedException ex) {
                Logger.getLogger(Runner.class.getName()).log(Level.SEVERE, null, ex);
            }
        } catch (IOException ex) {
            Logger.getLogger(Runner.class.getName()).log(Level.SEVERE, null, ex);
        }

        
        return ErrorCode.Err_0;
    }
}
