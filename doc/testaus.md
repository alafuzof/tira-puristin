# Mitä on testattu

Koodin toiminnallisuutta on testattu yksikkötesteillä, jotka voi itse ajaa


# Suorituskykytestaus




Huffman koodauksen pakkaussuhde vaihtelee pakattavan datan mukaan:

![Huffman compression ratio](/doc/images/huffman_compression_ratio.png)

Nähdään, että kemiallisten rakenteiden tietokanta nci pakkautuu parhaiten, jopa
alle kolmasosaan alkuperäisestä koosta. Sen sijaan vaihtelevasta binääridatasta
koostuva tähtitietokanta sao pakkautuu verrattain huonosti.

Myös Huffman pakkauksen ja purun ajallinen suorituskyky riippuu pakattavasta
datasta:

![Huffman compression performance](/doc/images/huffman_compression_performance.png)
![Huffman decompression performance](/doc/images/huffman_decompression_performance.png)
