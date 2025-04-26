#! /usr/bin/perl

foreach (<*.o>) {
	$file = $_;
	open(S, "nm6800 $_ |") || die;
	while (<S>) {
		chomp;
		if (/([\dA-F]{4}) C _(\w+)/) {
			$all{"$2:$file"} = $1;
			$ref{$2} = $1;
			$ref_f{$2} = $file;
		}
	}
	close S;
}

open(S, "a.map") || die;
while (<S>) {
	if (/([\dA-F]{4}) \w _?(\w+)/ && hex($1)) {
		$line{$1} = $2;
		$global{$2} = $1;
	}
}
close S;

foreach (keys %all) {
	s/:.*//;
	$ofs{$ref_f{$_}} = hex($global{$_}) - hex($ref{$_}) if $global{$_};
}

foreach (keys %all) {
	/:(.*)/;
	$adr = hex($all{$_}) + $ofs{$1};
	next unless $adr;
	$k = sprintf "%04X", $adr;
	$line{$k} = $_;
}

printf "%s %s\n", $_, $line{$_} foreach sort keys %line;

exit 0;
