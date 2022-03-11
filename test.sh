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
echo -e "\033[32;1m"\[NORM\]"\033[m"
read;
echo -e "\033[37;1m"INCLUDES"\033[m"
#read $a
cd includes/
norminette
echo -e "\n\033[37;1m"SOURCES"\033[m"
cd ../sources
norminette
cd ..

echo -e "\n\033[32;1m"\[MAKE\]"\033[m"
echo -e "\033[37;1m"make re"\033[m"
read;
make re
echo -e "\033[37;1m"check relink"\033[m"
read;
make
echo -e "\033[37;1m"make clean"\033[m"
read;
make clean
echo -e "\033[37;1m"make fclean"\033[m"
read;
make fclean
echo -e "\033[37;1m"make all"\033[m"
read;
make all

echo -e "\n\033[32;1m"\[MAP TEST\]"\033[m"
test_case=(`ls test_maps/*.fdf`)
read; clear
for testcase in ${test_case}
do
	echo map test
	echo "> cat ${testcase};./fdf ${testcase}"
	cat ${testcase};./fdf ${testcase}
	read; clear
done
