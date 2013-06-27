# Riows

Riows is a webserver built for easy deployment and installation that will handle static files and dynamic content.

## Little history

Riows is a re-write from scratch of my old webserver `rio` (https://github.com/felipecruz/rio) which is linux compatible, only, and
has many design flaws (although it does more than handle static files). When I started `rio` I wasn't prepared for creating such a
complex project but I did managed to create a good websocket parser library (https://github.com/felipecruz/cws) and a utf8 sequence
validator (https://github.com/felipecruz/utf8v) while developing it.

## Compatibility

Currently, it only works on **64 bits** systems and it's testes against: Linux, OSX and FreeBSD.

## Features

So far, Riows only handle static content and it'll serve the files on the working directory.

## Installing

It depends on `openssl`.

```sh
git clone https://github.com/felipecruz/riows.git && cd riows
git submodule init
git submodule update
make
sudo make install
```

## Running

```sh
sudo riows
```

## Testing

Riows has unit tests but still does not have functional tests.

### Running All Unit Tests

```sh
make ta
```

### Running specific module Unit Tests

```sh
make test module_name
```

`module_name` can be `rioev` or `rstatic`

## License

```
Copyright (c) 2013, Felipe João Pontes da Cruz
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
```
