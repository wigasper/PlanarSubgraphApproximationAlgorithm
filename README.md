# planarity-filter

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

```bash
$ ./build/planarityfilter -h
Arguments:
  -h [ --help ]         display help message
  -i [ --input ] arg    input file path
  -o [ --output ] arg   output file path
 ```

Note: Dockerfile has not been updated since the OpenMP dependency was added, 
this does not currently work
```bash
$ git clone https://github.com/wigasper/planarity-filter
$ cd planarity-filter
$ docker build -t planarityfilter:latest .
$ docker run -v /path/to/host/directory/:/bindmount_volume --rm -it planarityfilter:latest /planarityfilter -i /bindmount_volume/input_file -o /bindmount_volume/output_file -t 6
```
