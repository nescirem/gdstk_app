# gdstk_app
Application of [gdstk](https://github.com/heitzmann/gdstk)

## build

```sh
mkdir build
cd build
cmake ..
make gdstk_app
```

## usage

```sh
./gdstk_app -i source.gds -o target.oas
./gdstk_app -i source.db -b gds -o target.db -a oas
```
