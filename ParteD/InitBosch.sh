
./compileOS.sh
cp shell ./sfs_root
cp test12 ./sfs_root
#cp message.txt ./sfs_root
bochs -f opsys.bxrc