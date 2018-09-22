-- priest.lua

local priest = {}

-- Einige wunderbare S�tze auf Latein, die den "Wortschatz" unseres ehrw�rdigen Priesters bilden
priest.quotes = {
	"A cane non magno saepe tenetur aper.",
	"Aio, quantitas magna frumentorum est.",
	"Demum, veniunt porci!",
	"Feles mala! Cur cista non uteris? Stramentum novum in ea pousi.",
	"Imus ad magum Ozi videndum, magum Ozi mirum mirissimum.",
	"Ne auderis delere orbem rigidum meum!",
	"Tua mater tam antiqua ut linguam latinam loquatur",
}

function priest:interact (np, pl)
	--	Der default Dialog

	--	F�r den Spa� wird hier ein zuf�lliger Satz ausgew�hlt
	math.randomseed (os.time ())
	quote = priest.quotes[math.random((#priest.quotes))]

	TheGame:startDialog (quote, np, pl)
end

return priest
