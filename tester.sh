#!/bin/bash

make || exit 1
clear

g_ret=0
g_dir=logs

rm -rf $g_dir
mkdir -p $g_dir

n()
{
	local	dir

	if [[ "$1" == -* ]]; then
		dir=$g_dir/$1
	else
		dir=$g_dir/default
	fi

	mkdir -p $dir

	echo "Testing: ./ft_nm $@"

	nm "$@" 2>/dev/null > $dir/nm.txt
	local nm_ret=$?

	./ft_nm "$@" 2>/dev/null > $dir/ft_nm.txt
	local ft_nm_ret=$?

	if [ $nm_ret -ne $ft_nm_ret ]; then
		echo "❌ Return values differents (nm:$nm_ret vs ft_nm:$ft_ret)"
		let "g_ret+=1"
	fi

	if diff $dir/nm.txt $dir/ft_nm.txt > $dir/diff.txt; then
		echo "Success ✅"
		rm -rf $dir
	else
		echo "Fail ❌ (see $dir/diff.txt)"
		let "g_ret+=1"
	fi
}

options=(
	""
	"-a" "-u" "-g" "-p" "-r"
	"-au" "-ag" "-ap" "-ar"
	"-ug" "-up" "-ur"
	"-gp" "-gr"
	"-pr"
	"-aug" "-aup" "-aur" "-agp" "-agr" "-apr"
	"-ugp" "-ugr" "-upr"
	"-gpr"
	"-augp" "-augr" "-aupr" "-agpr"
	"-ugpr"
	"-augpr"
)

for opt in "${options[@]}"; do
	n $opt "$@"
done

if [ "$g_ret" -eq 0 ]; then
	echo "✅ All tests passed successfully"
	rm -rf $g_dir
elif [ "$g_ret" -eq 1 ]; then
	echo "❌" $g_ret "test failed / 32"
else
	echo "❌" $g_ret "tests failed / 32"
fi

exit "$g_ret"
