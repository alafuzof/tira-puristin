# Viikkoraportti 5

## Mitä tein?

Tällä viikolla Huffman-koodaus on tehty loppuun ja integroitu käyttöliittymään.
Komentoriviohjelmalla voi siis nyt Huffman-pakata ja -purkaa mielivaltaisia
tiedostoja. Kaikki tähän liittyvät tietorakenteet ja algoritmit on toteutettu
itse.

Lisäksi aloitin LZW-pakkauksen sanakirja tietorakenteen koodamista. Alunperin
suunniteltu hajautustauluratkaisu sai väistyä tehokkaamman ratkaisun tieltä.
Tämän osalta koodissa on toteutettu symbolijonojen haut sekä syötöt, mutta
testejä ei ole vielä kirjoitettu tälle koodille.

Kolmas isohko projekti tällä viikolla oli ensimmäinen vertaispalaute, joka oli
odotettua haastavampi minulle vieraan ohjelmointikielen (Go) vuoksi. Tämäkin
kuitenkin onnistui nähdäkseni ihan kunniallisesti.

## Miten ohjelma on edistynyt?

Huffman-koodaus on nyt oikeasti valmis, ja integroitu käyttöliittymään.
LZW-koodauksen keskeinen tietorakenne sanakirja (LZWDictionary) on aloitettu ja
ihan hyvässä vaiheessa.

## Mitä opin?

Löysin LZW-koodaukselle mielenkiintoisen vaihtoehtoisen toteutustavan
suomalaisen Juha "Warp" Niemisen sivuilta. Tämän toteutuksen pitäisi olla
suorituskyvyltään parempi kuin aiemmin suunniteltu hajautustaulumenettely.
Vertaisarviointia varten jouduin myös opiskelemaan Go-kielen perusteita ja
käännös-/testaustyökaluja

## Mikä jäi epäselväksi tai tuotti vaikeuksia?

Hankalinta tällä viikolla oli vertaisarviointi edellämainitun ei tutun
ohjelmointikielen vuoksi.

## Mitä teen seuraavaksi?

Seuraavalla viikolla on tarkoitus saattaa loppuun LZWDictionary sekä varsinainen
LZW-koodaus (jälkimmäisen pitäisi olla verrattain yksinkertaista, kun sanakirja
on valmis). Lisäksi olisi tarkoitus parantaa minulle annetussa
vertaisarvioinnissa mainittuja dokumentointiseikkoja sekä tehdä kunnollinen
suorituskykytestaus.
