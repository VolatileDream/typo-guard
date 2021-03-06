# typo-guard

A simple program to print out checksums for lines of characters.

## Usage

```
> typo-guard [--columns/-c <column-limit>]
```

`typo-guard` accepts input from `stdandard input`, and outputs to `standard
output`. `typo-guard` doesn't currently handle unicode.

When handling text with a terminal, `typo-guard` will continuously recompute
the checksum for a line, and output it with the line text.

## Format

```
<MurMurHash> " " <text>
```

[MurMurHash] is formatted as Base16.

Inspired by https://github.com/mpalmer/typo-guard

[MurMurHash]: https://en.wikipedia.org/wiki/MurmurHash
