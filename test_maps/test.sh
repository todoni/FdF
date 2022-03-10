#!/bin/zsh
#testcase_filename=("" "rt" ".rt" "qwe" "qwe.rt" "qwe.rt.qwe" "qwe.rt.qwe.rt")
#
#
#for testcase in ${testcase_filename}
#do
#	echo "> ./miniRT ${testcase}"
#	./miniRT ${testcase}
#done
#
clear
test_case=(`ls *.fdf`)
echo map test
read; clear
for testcase in ${test_case}
do
	echo map test
	echo "> cat ${testcase};./fdf ${testcase}"
	cat ${testcase};./fdf ${testcase}
	read; clear
done
