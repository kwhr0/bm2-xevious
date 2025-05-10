#! /usr/bin/perl

sub push_bits {
	my $d = $_[0];
	my $n = $_[1];
	my $i;
	for ($i = 0; $i < $n; $i++) {
		$tb = $tb << 1 | ($d >> $n - 1 & 1);
		$d <<= 1;
		if (++$tn == 16) {
			push(@d, $tb);
			$tb = 0;
			$tn = 0;
		}
	}
}

open(S, "map.bmp") || die;
binmode S;
read(S, $_, -s S);
close S;
@img = unpack("C*", $_);
$imgofs = $img[10] | $img[11] << 8;
$width = ($img[18] | $img[19] << 8) + 3 >> 2 << 2;
$height = $img[22] | $img[23] << 8;

for ($y = $height - 1; $y >= 0; $y--) {
	push(@ofs, $#d + 1);
	$d = 0;
	$l = 0;
	$c = 0;
	$tb = 0;
	$tn = 0;
	for ($x = 0; $x < $width; $x++) {
		$d = $img[$imgofs + $width * $y + $x] < 0x80;
		if ($l != $d) {
			$l = $d;
			if ($c > 1) {
				push_bits(1, 1);
				push_bits($c, 9);
			}
			else {
				push_bits(0, 1);
			}
			$c = 0;
		}
		$c++;
	}
	push_bits(1, 1);
	push_bits(0x1ff, 9);
	push_bits(0, 15); # flush last
}
open(O, "> bgdata.s") || die;
print O "\t.export\t_bgdata\n\t.code\n_bgdata:\n";
for ($i = 0; $i < @ofs; $i++) {
	printf O "\t.word\t0x%04x\n", $ofs[$i] + @ofs;
}
print O "; data\n";
for ($i = 0; $i < @d; $i++) {
	printf O "\t.word\t0x%04x\n", $d[$i];
}
close O;
open(O, "> bgconf.h") || die;
printf O "#define BG_XN\t%d\n#define BG_YN\t%d\n", $width, $height;
close O;
exit 0;
