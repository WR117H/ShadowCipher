void asciitohex(byte *ascii_ptr, byte *hex_ptr,int len)
{
    byte i,j,k;
    for(i = 0; i < len; i++)
    {
      // high byte first
      j = ascii_ptr[i] / 16;
      if (j>9) 
         { k = j - 10 + 65; }
      else 
         { k = j + 48; }
      hex_ptr[2*i] = k ;
      // low byte second
      j = ascii_ptr[i] % 16;
      if (j>9) 
         { k = j - 10 + 65; }
      else
         { k = j + 48; }
      hex_ptr[(2*i)+1] = k ; 
    };
    hex_ptr[(2*i)+2] = '\0' ; 
}

 void  hextoascii(byte *ascii_ptr, byte *hex_ptr,int len)
{
    byte i,j;
    for(i = 0; i < (len/2); i++)
     { 
     j = hex_ptr[i*2];
     if ((j>47) && (j<58))  ascii_ptr[i] = (j - 48) * 16;
     if ((j>64) && (j<71))  ascii_ptr[i] = (j - 55) * 16;
     if ((j>96) && (j<103)) ascii_ptr[i] = (j - 87) * 16;
     j = hex_ptr[i*2+1];
     if ((j>47) && (j<58))  ascii_ptr[i] = ascii_ptr[i]  + (j - 48);
     if ((j>64) && (j<71))  ascii_ptr[i] = ascii_ptr[i]  + (j - 55);
     if ((j>96) && (j<103)) ascii_ptr[i] = ascii_ptr[i]  + (j - 87);
     };
    ascii_ptr[i++] = '\0' ;
}
