/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.view;

import javafx.application.Application;
import static javafx.application.Application.launch;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 *
 * @author JFPS
 */
public class Main extends Application{
    @Override
    public void start(Stage stage) throws Exception {        
        FXMLLoader mainLoader = new FXMLLoader(getClass().getResource("MainFrm.fxml"));
        mainLoader.setController(new MainFrmController());
        
        Parent root = mainLoader.load();
        
        Scene scene = new Scene(root);
        stage.setTitle("PID Intensifies");
        
        stage.setScene(scene);
        stage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
}
