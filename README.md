# Installation

KMAN can be compiled and installed from the kman directory:
./configure; make ; sudo make install

# Running

The simplest alignment of fasta sequences can be performed with the command:

    kman -i input.fasta -o output_prefix -g

To perform alignment with features you need to first create a `.7c` file with
`convertFastaTo7c.py`.

KMAN command for aligning with features is:

    kman -i input.7c -o output_prefix -p 10 -d 3 -m 3 -c 7

For more detailed information try `kman --help`
