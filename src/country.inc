<?php
function get_country_codes(& $output)
{
  $xref[0xc0]='A';
  $xref[0xc1]='A';
  $xref[0xc2]='A';
  $xref[0xc3]='A';
  $xref[0xc4]='A';
  $xref[0xc5]='A';
  $xref[0xc6]='A';
  $xref[0xc7]='C';
  $xref[0xc8]='E';
  $xref[0xc9]='E';
  $xref[0xca]='E';
  $xref[0xcb]='E';
  $xref[0xcc]='I';
  $xref[0xcd]='I';
  $xref[0xce]='I';
  $xref[0xcf]='I';
  $xref[0xd0]='D';
  $xref[0xd1]='N';
  $xref[0xd2]='O';
  $xref[0xd3]='O';
  $xref[0xd4]='O';
  $xref[0xd5]='O';
  $xref[0xd6]='O';
  $xref[0xd8]='O';
  $xref[0xd9]='U';
  $xref[0xda]='U';
  $xref[0xdb]='U';
  $xref[0xdc]='U';
  $xref[0xdd]='Y';
  $xref[0xdf]='S'; // usually 2 esses but I am going to make this easy
  $xref[0xe0]='a';
  $xref[0xe1]='a';
  $xref[0xe2]='a';
  $xref[0xe3]='a';
  $xref[0xe4]='a';
  $xref[0xe5]='a';
  $xref[0xe6]='a';
  $xref[0xe7]='c';
  $xref[0xe8]='e';
  $xref[0xe9]='e';
  $xref[0xea]='e';
  $xref[0xeb]='e';
  $xref[0xec]='i';
  $xref[0xed]='i';
  $xref[0xee]='i';
  $xref[0xef]='i';
  $xref[0xf0]='d';
  $xref[0xf1]='n';
  $xref[0xf2]='o';
  $xref[0xf3]='o';
  $xref[0xf4]='o';
  $xref[0xf5]='o';
  $xref[0xf6]='o';
  $xref[0xf8]='o';
  $xref[0xf9]='u';
  $xref[0xfa]='u';
  $xref[0xfb]='u';
  $xref[0xfc]='u';
  $xref[0xfd]='y';
  $xref[0xff]='y';
  for($i=0;$i<128;$i++) $xref[$i]=chr($i);
  for($i=128;$i<192;$i++) $xref[$i]='?';
  //echo "set up xref<br>";
  $buffer=file_get_contents("http://www.iso.org/iso/list-en1-semic-3.txt");
  if($buffer) {
    //echo "got buffer full<br>";
    // translate any strange characters as best we can
    for($i=0;$i<strlen($buffer);$i++) {
      if(isset($xref[ord($buffer[$i])])) 
	$modified[$i]=$xref[ord($buffer[$i])];
      else 
	$modified[$i]='?';
    }
    unset($buffer);
    $buffer=explode("\n",implode($modified));
    //echo "exploded modified buffer<br>";
    //echo "there are " . count($buffer) . " pieces<br>";
    // the fist line is 'instruction' the next may be blank then the
    // country;2-char_code
    for($i=1;$i<count($buffer);$i++) {
      $tmp=trim($buffer[$i]);
      if(strlen($tmp)> 0) {
	$code=explode(";",$tmp);
        $output[$code[0]]=$code[1];
	//echo "$i. " . $code[0] . " --> " . $code[1] . "\n";
      }
    }
  }
  else {
    $output=false;
  }
  return;
}
?>
       
      
