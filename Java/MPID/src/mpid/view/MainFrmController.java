/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.view;

import java.io.File;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.ResourceBundle;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.geometry.Pos;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextInputDialog;
import javafx.scene.control.ToolBar;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.StackPane;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;

import mpid.codec.Decoder;
import mpid.codec.Decoder1;
import mpid.error.ErrorCode;
import mpid.io.Runner;

/**
 * FXML Controller class
 *
 * @author JFPS
 */
public class MainFrmController implements Initializable {

    private static final Logger LOG = Logger.getLogger("PID");

    @FXML
    private BorderPane borderPane;
    
    @FXML
    private ScrollPane scroll;
    @FXML
    private MenuBar menu;
    @FXML
    private StackPane stack;
    @FXML
    private Menu file;
    @FXML
    private MenuItem video;
    @FXML
    private MenuItem dir;
    
    private ImageView frames;
    private ToolBar toolbar;
    private Decoder dec = null;

    @Override
    public void initialize(URL location, ResourceBundle resources) {        
        initializeTools();
        initializeStack();
        initializeConsole();
    }
    
    private void initializeTools(){
        Image imagePrev = new Image(getClass().getResourceAsStream("/mpid/view/images/previous.png"));
        Button prev = new Button("", new ImageView(imagePrev));
        
        /*Image imagePause = new Image(getClass().getResourceAsStream("/mpid/view/images/pause.png"));
        Button pause = new Button("", new ImageView(imagePause));
        pause.setOnAction((ActionEvent e) -> {
            if (dec == null) return;
            playTask.cancel();
        });*/
        
        Image imagePlay = new Image(getClass().getResourceAsStream("/mpid/view/images/play.png"));
        Button play = new Button("", new ImageView(imagePlay));
        
        Image imageStop = new Image(getClass().getResourceAsStream("/mpid/view/images/stop.png"));
        Button stop = new Button("", new ImageView(imageStop));
        
        Image imageNext = new Image(getClass().getResourceAsStream("/mpid/view/images/next.png"));
        Button next = new Button("", new ImageView(imageNext));
        next.setOnAction((ActionEvent e) -> {
            if (dec == null) return;
            if (dec.hasFrames()){
                frames.setImage( dec.getNextFrame() );
                next.setDisable(false);
            } else {
                next.setDisable(true);
            }
            
        });
        
        stop.setOnAction((ActionEvent e) -> {
            if (dec == null) return;
            dec.reset();
            frames.setImage(dec.getNextFrame());
            next.setDisable(false);
        });
        
        prev.setOnAction((ActionEvent e) -> {
            if (dec == null) return;
            //if (dec.hasFrames())
                frames.setImage( dec.getPreviousFrame());
                next.setDisable(false);
        });
        
        play.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
                if (dec == null) return;
                /*Task task = new Task<Void>() {
                    @Override
                    public Void call() throws Exception {
                        System.out.println("ENTER");
                        while(dec.hasFrames()) {
                            System.out.println("WHILE");
                            Platform.runLater(() -> {
                                System.out.println("RUNE");
                                frames.setImage(dec.getNextFrame());
                            });
                        Thread.sleep(dec.getMilis());
                        }
                        return null;
                    }
                };
                Thread th = new Thread(task);
                th.setDaemon(true);
                th.start();
                playTask = task;
                System.out.println("PLAY!");
                System.out.println("MILIS: " + dec.getMilis());*/ //Problemas de sync
                prev.disableProperty().set(true);
                next.disableProperty().set(true);
                stop.disableProperty().set(true);
                new Timer().schedule(new TimerTask() {
                    @Override
                    public void run() {
                        while(dec.hasFrames()) {
                            frames.setImage(dec.getNextFrame());
                        }
                        this.cancel();
                        prev.disableProperty().set(false);
                        next.disableProperty().set(false);
                        stop.disableProperty().set(false);
                    }
                }, 0, dec.getMilis());        
            }
        });
        
        toolbar = new ToolBar(
            prev,
            //pause,
            play,
            stop,
            next
        );
        
        toolbar.setVisible(true);
        toolbar.setOpacity(0.3);
    }
    
    private void initializeStack(){
        //frames = new ImageView(new Image(getClass().getResourceAsStream("/mpid/view/images/Sample01_000.bmp")));
        frames = new ImageView();
        stack.getChildren().addAll(frames, toolbar);
        
        StackPane.setAlignment(toolbar, Pos.BOTTOM_RIGHT);
        stack.visibleProperty().set(true);
        //scroll.setContent(stack);
    }
    
    private void initializeConsole(){
        video.setOnAction((ActionEvent e) -> {
            FileChooser fileChooser = new FileChooser();
            fileChooser.setTitle("Escolha um arquivo de saída do Codec");
            fileChooser.getExtensionFilters().add(
                new ExtensionFilter("PID Files", "*.tvf")
            );
            File selectedFile = fileChooser.showOpenDialog(borderPane.getScene().getWindow());
            if (selectedFile != null) { //https://stackoverflow.com/questions/25491732/how-do-i-open-the-javafx-filechooser-from-a-controller-class
                String capped = selectedFile.getName().split("\\.")[0];
                int cap;
                
                try{
                    cap = Integer.parseInt(capped);
                    dec = new Decoder1(selectedFile, Integer.parseInt(capped));
                    frames.setImage(dec.getNextFrame());
                } catch (NumberFormatException ex){
                    //https://docs.oracle.com/javase/8/javafx/api/javafx/scene/control/Alert.html
                    Alert alert = new Alert(AlertType.ERROR, "Nome do arquivo não apresenta o framerate. "
                            + "É necessário devido a limitações da codificação (Ex.: 30.tvf para um com 30 FPS");
                    alert.show();
                    //This is some nasty coding right here... :x
                    //Codification limitations
                }    
            }
        });
      
        dir.setOnAction((ActionEvent e) -> {
            DirectoryChooser dirChooser = new DirectoryChooser();
            dirChooser.setTitle("Escolha um arquivo de saída do Codec");
            
            File selectedFile = dirChooser.showDialog(borderPane.getScene().getWindow());
            if (selectedFile != null) {
                //System.out.println("Selected File: " + selectedFile);
                //System.out.println(selectedFile.getName());
                //System.out.println(selectedFile.isDirectory());
                //String capped = selectedFile.getName().split("");
                
                File[] listOfFiles = selectedFile.listFiles();
                List<File> arquivosParaCodificar = new ArrayList();
                for (File arq : listOfFiles){
                    if (arq.getName().contains(".bmp"))
                        arquivosParaCodificar.add(arq);
                }
                //https://stackoverflow.com/questions/8309981/how-to-create-and-show-common-dialog-error-warning-confirmation-in-javafx-2
                TextInputDialog diag = new TextInputDialog("25");
                diag.setContentText(arquivosParaCodificar.size() + " arquivos bmp foram descobertos.\n"
                        + "Qual o framerate desejado? O nome do arquivo será o framerate, devido à limitações no formato");
                Optional<String> res = diag.showAndWait();
                if (res.isPresent()){
                    String outName = res.get() + ".tvf";
                    Runner r = new Runner(arquivosParaCodificar, selectedFile, outName);
                    ErrorCode error = r.run();
                    
                    if (error == ErrorCode.Err_0){
                        Alert alert = new Alert(AlertType.INFORMATION);
                        alert.setTitle("Sucesso");
                        alert.setHeaderText("Operação realizada com sucesso");
                        alert.setContentText("Observação: Tempo extra foi necessário para cópia do arquivo de \"vídeo\"");
                        alert.show();
                    } else {
                        Alert alert = new Alert(AlertType.ERROR);
                        alert.setTitle("Erro");
                        alert.setHeaderText(error.message);
                        alert.show();
                    }
                    
                    //System.out.println(r.run());
                }
            }
        });
        
    }
    
}
