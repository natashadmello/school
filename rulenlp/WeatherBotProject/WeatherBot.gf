abstract WeatherBot = {
  flags startcat = Facts ;

  cat
    Facts ; City ; WeatherCondition ; Num ; TempIndex ; HumIndex ; When ; TemperatureType ; Unknown ; Variants ;
   
  fun
       
    QWeatherFact : Variants -> City -> When -> Facts ;
    AWeatherFact : City -> When -> Variants -> WeatherCondition -> Facts ;

    QTemperatureFact : Variants -> City -> When -> TemperatureType -> Facts ;
    ATemperatureFact : City -> When -> Variants -> TempIndex -> TemperatureType -> Facts ;
    
    QHumidityFact : Variants -> City -> When -> Facts ; 
    AHumidityFact : City -> When -> Variants -> HumIndex -> Facts ; 
    
    QForecastFact : Variants -> City -> When -> Facts ;
    AForecastFact : City -> When -> Variants -> WeatherCondition -> TempIndex -> TemperatureType -> Facts ;
    
    TIndex : Num -> TempIndex ;
    HIndex : Num -> HumIndex ;
    WNum: Float -> Num ;
    
    QUnknown: Unknown -> City -> When -> Facts;

-- Lexicon
    welcomeStart, qInvalid: Facts ;
    Paris, NewYork, Tampere, Helsinki : City ;
    Sunny, Cloudy, Windy, Rainy, Snowy, ThunderStorm, ClearSky : WeatherCondition ;
    Today, Tomorrow, DayAfter, NextWeek, Yesterday, RightNow : When ;
    Fah, Cel : TemperatureType ;
    UnknownTemp, UnknownWeather, UnknownHumid, UnknownForecast : Unknown ;
    Is, Was, WillBe : Variants ;
    
            
}