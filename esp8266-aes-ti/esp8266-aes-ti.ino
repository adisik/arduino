#include <Arduino.h>
#include <aes_ti.h>
#include <ebase64.h>

#define BLOCK_SIZE 16 // AES 128bit / 8 bits = 16 bytes

// helper method for printing array of bytes
void printArray(String name, unsigned char*arr, int length)
{
    String hx = name + " (hex): ";
    Serial.print(name + ": ");
    for (int i = 0; i < length; i++)
    {
        Serial.write(arr[i]);
        hx += String(arr[i], HEX) + ":";
    }
    Serial.println();
    Serial.println(hx);
}

void testAES128()
{
    char data[1024];
    strcpy(data, "{misamisamisamisami}");

    unsigned char key [] = {'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'};

    char cipherb64[16 * 4];
  
    Serial.println("===== TEST AES 128 ====="); 

    printArray("Plain", (unsigned char*)data, strlen(data));

    unsigned int pkcs7size = paddingPKCS7((unsigned char*)data, strlen(data), BLOCK_SIZE);

    printArray("Plain", (unsigned char*)data, pkcs7size);
    printArray("Key", key, sizeof(key));
    Serial.println("PKCS7Size" + String(pkcs7size));

    for (int i = 0; i < pkcs7size; i += BLOCK_SIZE)
    {
        Serial.println("Encrypt at pos " + String(i));
        printArray("Encrypting block", (unsigned char*)(data + i), BLOCK_SIZE);
        ti_aes_encrypt((unsigned char*)(data + i), key);
        printArray("Encrypted block", (unsigned char*)(data + i), BLOCK_SIZE);
    }

    printArray("Encrypted", (unsigned char*)data, pkcs7size);

    // BASE64 encode ciphered message
    byte cipherb64len = base64_encode(cipherb64, data, pkcs7size);
    Serial.println("Encrypted message in B64: " + String(cipherb64)); 
}

/*
Padding of buffer according to PKCS7 scheme
https://en.wikipedia.org/wiki/Padding_(cryptography)

Be carefull, buffer needs to be large enough since padded content always
occupies more space than original buffer.  (1-block-size)
*/
unsigned int paddingPKCS7(unsigned char *buffer, unsigned int content_size, unsigned char block_size)
{
    // compute how many padding bytes are necessary
    // get number of bytes missing in last block
    // even if content is aligned with block_size, we need to add
    // one special block (according to PKCS7 standard)
    unsigned char padding_size = block_size - (content_size % block_size);

    Serial.println(String(content_size) + ':' + String(block_size) + ':' + String(padding_size));

    // set those bytes in buffer
    memset(buffer + content_size, padding_size, padding_size);

    // return final size of the content in PKCS7 encoding
    return content_size + padding_size;
}



/********************************************************************/
void setup(void)
{
    Serial.begin(9600);
}
void loop(void)
{
    testAES128();
    delay(20000);
}


