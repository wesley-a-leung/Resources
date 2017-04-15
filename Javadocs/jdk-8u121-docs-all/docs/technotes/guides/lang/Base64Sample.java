/*
* Copyright (c) 2013, Oracle and/or its affiliates. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*
*   - Neither the name of Oracle or the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
* IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
     
import java.net.URL;
import java.util.Base64;
import java.util.Base64.Encoder;
import java.util.Base64.Decoder;

class Base64Sample {
    public static void main(String args[]){
        try{
            URL myurl = new URL("http://example.com");
            Base64.Encoder urec = Base64.getUrlEncoder();
            System.out.println("URL: " + myurl);
            String str = "thisIsAnExampleString";
            Base64.Encoder enc= Base64.getEncoder();
            
            //encoding  byte array into base 64
                
            byte[] encoded = urec.encode(myurl.toString().getBytes("UTF8"));
            byte[] strenc =enc.encode(str.getBytes("UTF-8"));
            
            System.out.println("Base64 Encoded URL : " + new String(encoded,"UTF-8"));
            System.out.println("Base64 Encoded String : " + new String(strenc,"UTF-8"));
        
            //decoding byte array into base64
            Base64.Decoder urdc= Base64.getUrlDecoder();
            Base64.Decoder dec= Base64.getDecoder();
            byte[] urdec=urdc.decode(encoded);
            byte[] strdec=dec.decode(strenc);
            
            System.out.println("Base64 Decoded URL : " + new String(urdec,"UTF-8"));
            System.out.println("Base64 Decoded String : " + new String(strdec,"UTF-8"));
        }

        catch(Exception e){
            System.out.println("Invalid URL Exception");
        }
    }
}


    
   
