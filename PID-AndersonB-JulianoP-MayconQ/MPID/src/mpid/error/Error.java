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
public class Error {
    private final ErrorCode[] BUILDER;

    public Error() {
        BUILDER = new ErrorCode[]{
            ErrorCode.Err_0,
            ErrorCode.Err_1,
            ErrorCode.Err_2,
            ErrorCode.Err_3,
            ErrorCode.Err_4,
            ErrorCode.Err_5,
            ErrorCode.Err_6,
        };
    }
    
    public ErrorCode getError(int errorCode){
        if (errorCode == -1){
            return ErrorCode.Err_null;
        }
        
        if (errorCode >= BUILDER.length){
            return ErrorCode.Err_null;
        }
        
        return BUILDER[errorCode];
    }
    
    
}
