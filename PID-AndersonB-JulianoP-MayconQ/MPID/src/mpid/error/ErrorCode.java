/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mpid.error;

/**
 *
 * @author Juliano_Felipe
 */
public enum ErrorCode {  
    Err_null(-1, "Erro não catalogado"),
    Err_0(0, "All good"),
    Err_1(1, "Quantidade de parâmetros inválida"),
    Err_2(2, "Arquivo de entrada não encontrado"),
    Err_3(3, "Arquivo de saída não pode ser aberto"),
    Err_4(4, "Arquivo sem assinatura de bitmap"),
    Err_5(5, "Imagem com paleta"),
    Err_6(6, "Imagem com dimensões maiores que 4095 não são suportadas por esta versão");
    
    public final int code;
    public final String message;

    
    private ErrorCode(int code, String message) {
        this.code = code;
        this.message = message;
    }
        
}
