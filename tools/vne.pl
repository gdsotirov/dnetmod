#!/usr/bin/perl

if ( scalar(@ARGV) < 1 ) {
    print("Vendor Names Extractor v0.1 by GDS ;-)");
    print("usage: $0 <filename>\n");
    exit(-1);
}

if ( !open(VENDFILE, '<', @ARGV) ) {
   print("Can't open file ".@ARGV."!\n");
   exit(-2);
}

$Index = 0;
while ( <VENDFILE> ) {
   $/ = "\n"; chomp();
   $/ = "\r"; chomp();
   ($VendID, $VendName) = split(/\t/);
   while ( $VendID > $Index ) {
       printf("    /* %03d */   \"Reserved\",\n", $Index);
       $Index++;
   }
   if ( $VendID == $Index ) {
       printf("    /* %03d */   \"%s\",\n", $VendID, $VendName);
   }
   $Index++;
}

close(VENDFILE);

exit(0);

