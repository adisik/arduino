#include <Arduino.h>
#include <aes_ti.h>
#include <ebase64.h>

// helper method for printing array of bytes
void printArray(String name, byte *arr, int length)
{
    String hx = name + " (hex): ";
    Serial.print(name + ": ");
    for (int i = 0; i < length; i++)
    {
        Serial.write(arr[i]);
        hx += String(arr[i], HEX);
    }
    Serial.println();
    Serial.println(hx);
}

void testAES128()
{
    unsigned char state[] = {'{', 'm', 'i', 's', 'a', 'm', 'i', 's', 'a', 'm', 'i', 's', 'a', 'm', 'i', '}'};

    unsigned char key [] = {'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'};

    char cipherb64[16 * 4];
  
    Serial.println("===== TEST AES 128 ====="); 

    printArray("Plain", state, sizeof(state));

    paddingPKCS7(state);

    printArray("Plain", state, sizeof(state));
    printArray("Key", key, sizeof(key));
   
    ti_aes_encrypt(state,key);

    printArray("Encrypted", state, sizeof(state));

    // BASE64 encode ciphered message
    byte cipherb64len = base64_encode(cipherb64, (char*)state, 16);
    Serial.println("Encrypted message in B64: " + String(cipherb64)); 
}

/*
Padding of buffer according to PKCS7 scheme
https://en.wikipedia.org/wiki/Padding_(cryptography)
*/
void paddingPKCS7(unsigned char *buffer, unsigned int content_size)
{
    //unsigned int content_size % 
}

/********************************************************************/
void setup(void)
{
    Serial.begin(9600);
}
void loop(void)
{
    testAES128();
    delay(10000);
}


