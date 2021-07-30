program repeat1 (input,output);
var i,j : integer;
   begin
      i:=0; j:=0;
      repeat
         repeat
            write(i,j);
            j:=j+1;
         until j>12;
         i:=i+1;
      until i>12;
   end.
