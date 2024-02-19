calcD5 : calc_d5.cc check_mask.cc 
	/usr/local/hdf5/bin/h5c++ -o calcD5 calc_d5.cc check_mask.cc  -I./include  -std=c++11
