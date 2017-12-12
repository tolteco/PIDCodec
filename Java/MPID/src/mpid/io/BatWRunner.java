/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.io;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;

///'##::::'##:'####::'######::::::'######::'##::::'##:'##::: ##:'########:
/// ##:::: ##:. ##::'##... ##::::'##... ##: ##:::: ##: ###:: ##:... ##..::
/// ##:::: ##:: ##:: ##:::..::::: ##:::..:: ##:::: ##: ####: ##:::: ##::::
/// #########:: ##:: ##::::::::::. ######:: ##:::: ##: ## ## ##:::: ##::::
/// ##.... ##:: ##:: ##:::::::::::..... ##:. ##:: ##:: ##. ####:::: ##::::
/// ##:::: ##:: ##:: ##::: ##::::'##::: ##::. ## ##::: ##:. ###:::: ##::::
/// ##:::: ##:'####:. ######:::::. ######::::. ###:::: ##::. ##:::: ##::::
///..:::::..::....:::......:::::::......::::::...:::::..::::..:::::..:::::
///'########::'########:::::'###:::::'######:::'#######::'##::: ##:'########::'######::
/// ##.... ##: ##.... ##:::'## ##:::'##... ##:'##.... ##: ###:: ##: ##.....::'##... ##:
/// ##:::: ##: ##:::: ##::'##:. ##:: ##:::..:: ##:::: ##: ####: ##: ##::::::: ##:::..::
/// ##:::: ##: ########::'##:::. ##: ##::::::: ##:::: ##: ## ## ##: ######:::. ######::
/// ##:::: ##: ##.. ##::: #########: ##::::::: ##:::: ##: ##. ####: ##...:::::..... ##:
/// ##:::: ##: ##::. ##:: ##.... ##: ##::: ##: ##:::: ##: ##:. ###: ##:::::::'##::: ##:
/// ########:: ##:::. ##: ##:::: ##:. ######::. #######:: ##::. ##: ########:. ######::

public class BatWRunner {
    private boolean BATStatus;
    
    private BatWRunner(){
        
    }
    
    public BatWRunner(String link){
        BATStatus = writeBat (link);
    }
    
    public boolean status(){
        return BATStatus;
    }
    
    private boolean writeBat (String link){
        final File file = new File("LPHandlerTMP.bat");
        try {
            file.createNewFile();
        } catch (IOException ex) {
            Logger.getLogger("PID").log(Level.SEVERE, null, ex);
            return false;
        } //Finalização de criação de arquivo
        
        PrintWriter writer = null;
        try {
            writer = new PrintWriter(file, "UTF-8");
        } catch (FileNotFoundException | UnsupportedEncodingException ex) {
            Logger.getLogger("PID").log(Level.SEVERE, null, ex);
            return false;
        } //Finalização de inicialização de escritor

        writer.println("@echo off");
        writer.println("::TODO: Non hardcoded firefox path.");
        writer.println("cd \"C:\\Program Files\\Mozilla Firefox\"");
        //writer.println("tasklist /FI \"IMAGENAME eq firefox.exe\" 2>NUL | find /I /N \"firefox.exe\">NUL");
        //writer.println("if \"%ERRORLEVEL%\"==\"0\" goto opentab");
        //writer.println("if \"%ERRORLEVEL%\"==\"1\" goto openPrivateFox");
        //writer.println(":openPrivateFox   ::Firefox not currently opened");
        //writer.println("start firefox.exe -private");
        //writer.println("start firefox.exe -private-window");
        //writer.println(":opentab          ::Firefox already opened");
        writer.println("start firefox.exe -private-window \"" + link + "\"");
        writer.println("exit /b 0");
        writer.close();
        //Finalização de criação de comandos
        
        boolean execution = execBat (file);
        if (!execution){
            JOptionPane.showMessageDialog(null, "Erro ao executar/excluir .bat temporário. Func. execBat.", "Erro", JOptionPane.ERROR_MESSAGE);
            return false;
        }
        
        return true;
    }
    
    private boolean execBat (File bat) {
        try {
            Process runBat = Runtime.getRuntime().exec("cmd /c " + bat);
            runBat.waitFor();
            bat.delete();
        } catch (IOException | InterruptedException ex) {
            Logger.getLogger("PID").log(Level.SEVERE, null, ex);
            return false;
        } //Finalização de execução de arquivo
        
        return true;
    }
}