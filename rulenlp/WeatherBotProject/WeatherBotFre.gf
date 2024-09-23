-- I have used Google translate to translate words from English to French, so grammar might not be correct.
-- The letters with acute accents won't be printed because of lexical errors so I tried adding щи in front of them, but that didn't work either.
-- So I replaced them with english alphabets.

concrete WeatherBotFre of WeatherBot = 
open Predef, SymbolicEng
in
{

  lincat
    Facts, City, WeatherCondition, When, TempIndex , HumIndex, TemperatureType, Unknown, Variants = {s : Str} ;
    Num = {sym : Symb} ;
    
  lin
    welcomeStart = {s = "Bonjour ! Comment puis-je vous aider aujourd'hui?"} ;
    qInvalid = {s = "Je ne comprends pas ce que vous voulez dire. Veuillez poser une question valide."} ;
    Paris = {s = "Paris"} ;
    NewYork = {s = "New York"} ;
    Tampere = {s = "Tampere"} ;
    Helsinki = {s = "Helsinki"} ;
    Sunny = {s = "ensoleille"} ;
    Cloudy = {s = "nuageux"} ;
    Windy = {s = "venteux"} ;
    Rainy = {s = "pluvieux"} ;
    Snowy = {s = "neigeux"} ;
    ThunderStorm = {s = "orageux"} ;
    ClearSky = {s = "ciel degage"} ;
    Today = {s = "aujourd'hui"} ;
    RightNow = {s = "en ce moment"} ;
    Tomorrow = {s = "demain"} ;
    DayAfter = {s = "apres-demain"} ;
    NextWeek = {s = "la semaine prochaine"} ;
    Yesterday = {s = "hier"} ;
    Fah = {s = "Fahrenheit"} ;
    Cel = {s = "Celsius"} ;
    UnknownTemp = {s = "la temperature"} ;
    UnknownWeather = {s = "les conditions meteorologiques"} ;
    UnknownHumid = {s = "le niveau d'humidite"} ;
    UnknownForecast = {s = "les previsions meteorologiques"} ;
    Is = {s = "est"} ;
    Was = {s = "etait"} ;
    WillBe = {s = "sera"} ;

                
    
    QWeatherFact variant city time =
      { s = "Quel" ++ variant.s ++ "le temps a" ++ city.s ++ time.s ++"?"}
    | { s = "Quel" ++ variant.s ++ "le temps a" ++ city.s ++ time.s} 
    | { s = "Comment" ++ variant.s ++ "le temps a" ++ city.s ++ time.s ++ "?"}
    | { s = "Comment" ++ variant.s ++ "le temps a" ++ city.s ++ time.s}
    ;

   AWeatherFact city time variant wc =    
      { s = "Le temps a" ++ city.s ++ time.s ++ variant.s ++ wc.s}
    | { s = "A" ++ city.s ++ "le temps" ++ time.s ++ variant.s ++  wc.s} 
    ;
			
    QTemperatureFact variant city time tt = 
      { s = "Quelle" ++ variant.s ++ "la temperature a" ++ city.s ++ time.s ++ "en" ++ tt.s ++"?"}
    | { s = "Quelle" ++ variant.s ++ "la temperature a" ++ city.s ++ time.s ++ "en" ++ tt.s}    
    | { s = "Quelle" ++ variant.s ++ "la temperature a" ++ city.s ++ time.s ++"?"}    
    | { s = "Quelle" ++ variant.s ++ "la temperature a" ++ city.s ++ time.s}
    ;
    
    ATemperatureFact city time variant temp tt =
      { s = "La temperature a" ++ city.s ++ time.s ++ variant.s ++ temp.s ++ "degres"++ tt.s}
    | { s = "La temperature a" ++ city.s ++ time.s ++ variant.s++ temp.s ++ "degres" ++ tt.s}
    ;

    QHumidityFact variant city time = 
      { s = "Quel" ++ variant.s ++ "le niveau d'humidite a" ++ city.s ++ time.s ++ "?"}
    | { s = "Quel" ++ variant.s ++ "le niveau d'humidite a" ++ city.s ++ time.s}
    ;
    
    AHumidityFact city time variant hind = 
      { s = "Le niveau d'humidite a" ++ city.s ++ time.s ++ variant.s ++ hind.s ++ "pour cent."}
    | { s = "Le niveau d'humidite" ++ time.s ++ variant.s ++ hind.s ++ "pour cent."}    ;

    QForecastFact variant city time =
      { s = "Quelle" ++ variant.s ++ "la prevision meteo pour" ++ city.s ++ time.s ++ "?"}
    | { s = "Quelle" ++ variant.s ++ "la prevision meteo pour" ++ city.s ++ time.s} 
    ;

    AForecastFact city time variant wc temp tt = { s = "La prevision meteo a" ++ city.s ++ time.s ++ variant.s ++ wc.s ++ "avec une temperature de" ++ temp.s ++ "degres"  ++ tt.s} ;

    TIndex n = {s = n.sym.s} ;
    HIndex n = {s = n.sym.s} ;

        
    WNum f = {sym = mkSymb f.s} ;


    QUnknown u city time = {s = "Je ne connais pas" ++ u.s ++ "a" ++ city.s ++ time.s} ;
}
