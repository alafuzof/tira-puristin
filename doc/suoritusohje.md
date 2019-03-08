# Kääntämiseen tarvittavat ohjelmistot

Koodi on käännettävissä Linux-alustalla, johon on asennettu verrattain tuoreet
(esim. Ubuntu 18.04) versiot seuraavista ohjelmistoista

- CMake
- Git
- GCC
- Doxygen (to build documentation)

# Kääntäminen

Siirry build-kansioon ja aja seuraavat komennot:

```
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_PERF=True ..
make
```

# Ohjelman ajaminen

Kääntämisen jälkeen build-kansiossa pitäisi olla `puristin`-niminen ohjelma.
Ohjelma näyttää ohjetulosteen ajettaessa ilman lisäparametreja. Käyttö pitäisi
olla suoraviivaista ohjeen mukaan.

# Testien ajaminen

Kääntämisen jälkeen build-kansiossa pitäisi olla `puristin_test`- ja
`puristin_perf`-nimiset testiohjelmat. `puristin_test` ajaa yksikkötestit ja
`puristin_perf` suorituskykytestit nk. dickens aineistolla, jonka cmake on
ladannut build/test_data kansioon.
