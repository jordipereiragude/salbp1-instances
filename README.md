# SALBP-1 Instances 

Instances generated for 

Álvarez-Mirada, E., Pereira, J., Vilà, M., Analysis of the simple assembly line balancing problem complexity, published in Computers and Operations Research.

DOI: 10.1016/j.cor.2023.106323

Contents:

- Folder src: Contains instance generator in C. It also includes a makefile and a file to test Order strength. Before running make you have to create an obj folder in the root directory.

- Folder script: Script used to generate all instances and hash value reported by execution of "md5 $file" and "shasum -a 256 $filename" on each file.

The script generates folders and files according to characteristics outlined in the paper and in the command line of each execution of the procedure. 

If you have any issues with the instances, please contact the corresponding author of the paper: jordi.pereira at bsm.upf.edu

Changes:

July 15th, 2024. In a recently installed computer, the compiler complained for math library. It has now been added to the Makefile. Also, the contact information has been updated
