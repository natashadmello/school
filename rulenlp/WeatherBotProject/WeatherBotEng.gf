concrete WeatherBotEng of WeatherBot = 
open Predef, SymbolicEng
in
{

  lincat
    Facts, City, WeatherCondition, When, TempIndex , HumIndex, TemperatureType, Unknown, Variants = {s : Str} ;
    Num = {sym : Symb} ;
    
  lin
    welcomeStart = {s = "Hello! How can I assist you today?"} ;
    qInvalid = {s = "I don't know what you mean. Please ask a valid question."} ;
    Paris = {s = "Paris"} ;
    NewYork = {s = "New York"} ;
    Tampere = {s = "Tampere"} ;
    Helsinki = {s = "Helsinki"} ;
    Sunny = {s = "sunny"} ;
    Cloudy = {s = "cloudy"} ;
    Windy = {s = "windy"} ;
    Rainy = {s = "rainy"} ;
    Snowy = {s = "snowy"} ;
    ThunderStorm = {s = "thunderstorm"} ;
    ClearSky = {s = "clear sky"} ;
    Today = {s = "today"} ;
    RightNow = {s = "right now"} ;
    Tomorrow = {s = "tomorrow"} ;
    DayAfter = {s = "day after tomorrow"} ;
    NextWeek = {s = "next week"} ;
    Yesterday = {s = "yesterday"} ;
    Fah = {s = "Fahrenheit"} ;
    Cel = {s = "Celsius"} ;
    UnknownTemp = {s = "the temperature"} ;
    UnknownWeather = {s = "the weather conditions"} ;
    UnknownHumid = {s = "the humidity level"} ;
    UnknownForecast = {s = "the weather forecast"} ;
    Is = {s = "is"} ;
    Was = {s = "was"} ;
    WillBe = {s = "will be"} ;
                
    
    QWeatherFact variant city time =
      { s = "What" ++ variant.s ++ "the weather in" ++ city.s ++ time.s ++"?"}
    | { s = "What" ++ variant.s ++ "the weather in" ++ city.s ++ time.s} 
    | { s = "How" ++ variant.s ++ "the weather in" ++ city.s ++ time.s ++ "?"}
    | { s = "How" ++ variant.s ++ "the weather in" ++ city.s ++ time.s}
    ;
    

    AWeatherFact city time variant wc =
      { s = "The weather in" ++ city.s ++ time.s ++ variant.s ++ wc.s}
    | { s = "In" ++ city.s ++ "the weather" ++ variant.s ++  wc.s} 
    ;
			
    QTemperatureFact variant city time tt = 
      { s = "What" ++ variant.s ++ "the temperature in" ++ city.s ++ time.s ++ "in" ++ tt.s ++"?"}
    | { s = "What" ++ variant.s ++ "the temperature in" ++ city.s ++ time.s ++ "in" ++ tt.s}
    | { s = "What" ++ variant.s ++ "the temperature in" ++ city.s ++ time.s ++"?"}
    | { s = "What" ++ variant.s ++ "the temperature in" ++ city.s ++ time.s}
    ;
    

    ATemperatureFact city time variant temp tt = 
      { s = "Its temperature" ++ variant.s ++ temp.s ++ "degrees" ++ tt.s}
    | { s = "The temperature in" ++ city.s ++ time.s ++ variant.s ++ temp.s ++ "degrees" ++ tt.s}
    ;

    QHumidityFact variant city time = 
      { s = "What" ++ variant.s ++ "the humidity level in" ++ city.s ++ time.s ++ "?"}
    | { s = "What" ++ variant.s ++ "the humidity level in" ++ city.s ++ time.s}
    ;
    
    AHumidityFact city time variant hind = 
      { s = "The humidity level in" ++ city.s ++ time.s ++  variant.s ++ hind.s ++ "percent."}
    | { s = "Its humidity level" ++ time.s ++ variant.s ++ hind.s ++ "percent."}
    ;
    
    QForecastFact variant city time = 
      { s = "What" ++ variant.s ++ "the weather forecast for" ++ city.s ++ time.s ++ "?"}
    | { s = "What" ++ variant.s ++ "the weather forecast for" ++ city.s ++ time.s} 
    ;

    AForecastFact city time variant wc temp tt = { s = "The weather forecast in" ++ city.s ++ time.s ++ variant.s ++ wc.s ++ "with a temperature of" ++ temp.s ++ "degrees" ++ tt.s} ;
  

    TIndex n = {s = n.sym.s} ;
    HIndex n = {s = n.sym.s} ;
   
    WNum f = {sym = mkSymb f.s} ;

    QUnknown u city time = {s = "I don't know" ++ u.s ++ "in" ++ city.s ++ time.s} ;
}
