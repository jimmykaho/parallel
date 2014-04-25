// While not particulary secure... it turns out this strategy also isn't too 
// bad (as long as the keys are kept secret and have different lengths and 
// aren't reused together)... or at least that is what a member of the security
// group that works in crypto told me.

#include "key.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <tbb/tbb.h>

// utility function: given a list of keys, a list of files to pull them from, 
// and the number of keys -> pull the keys out of the files, allocating memory 
// as needed
void getKeys(xorKey* keyList, char** fileList, int numKeys)
{
  int keyLoop=0;
  for(keyLoop=0;keyLoop<numKeys;keyLoop++)
  {
     readKey(&(keyList[keyLoop]), fileList[keyLoop]);
  }
}

//Given text, a list of keys, the length of the text, and the number of keys, encodes the text
void encode(char* plainText, char* cypherText, xorKey* keyList, int ptextlen, int numKeys) {
 
  tbb::parallel_for (
    tbb::blocked_range<int> ( 0, ptextlen ),
    // use the [=] lambda expression to capture any referenced variable by making a copy
    [=](tbb::blocked_range<int> r) {
      for( int charLoop = r.begin(); charLoop < r.end(); ++charLoop ) {
        char cipherChar = plainText[charLoop];
        char res = tbb::parallel_reduce(
          tbb::blocked_range<int>( 0, numKeys ),
          char(0),
          // use the [=] lambda expression to capture any referenced variable by making a copy
          [=]( const tbb::blocked_range<int>& r, char value )->char {
            for( int reduer_index=r.begin(); reduer_index!=r.end(); ++reduer_index ){
              value = value ^ getBit( &(keyList[reduer_index]), charLoop );
            }
            return value;
          },
//                std::bit_xor<char>()
          // try this
          [=](char x, char y)->char{
            return x^y;
          }
        );
      cipherChar = cipherChar ^ res;
      cypherText[charLoop]=cipherChar;
    }
  });
}

void decode(char* cypherText, char* plainText, xorKey* keyList, int ptextlen, int numKeys) {
  encode(cypherText, plainText, keyList, ptextlen, numKeys); //isn't symmetric key cryptography awesome? 
}

int main(int argc, char* argv[]) {
  if(argc<=2)
  {
      printf("Usage: %s <fileToEncrypt> <key1> <key2> ... <key_n>\n",argv[0]);
      return 1;
  }

  // read in the keys
  int numKeys=argc-2;
  xorKey* keyList=(xorKey*)malloc(sizeof(xorKey)*numKeys); // allocate key list
  getKeys(keyList,&(argv[2]),numKeys);
  
  // read in the data to encrypt/decrypt
  off_t textLength=fsize(argv[1]); //length of our text
  FILE* rawFile=(FILE*)fopen(argv[1],"rb"); //The intel in plaintext
  char* rawData = (char*)malloc(sizeof(char)*textLength);
  fread(rawData,textLength,1,rawFile);
  fclose(rawFile);

  // Encrypt
  char* cypherText = (char*)malloc(sizeof(char)*textLength);
  encode(rawData,cypherText,keyList,textLength,numKeys);

  // Decrypt
  char* plainText = (char*)malloc(sizeof(char)*textLength);
  decode(cypherText,plainText,keyList,textLength,numKeys);

  // write out
  FILE* encryptedFile=(FILE*)fopen("encryptedOut","wb");
  FILE* decryptedFile=(FILE*)fopen("decryptedOut","wb");

  fwrite(cypherText,textLength,1,encryptedFile);
  fwrite(plainText,textLength,1,decryptedFile);

  fclose(encryptedFile);
  fclose(decryptedFile);

  // Check
  int i;
  for(i=0;i<textLength;i++) {
    if(rawData[i]!=plainText[i]) {
      printf("Encryption/Decryption is non-deterministic\n");
      i=textLength;
    }
  }

  return 0;

}
