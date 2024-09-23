# WeatherForecast Chat System

This is a README for the WeatherForecast Chat System. The system is designed to provide weather forecasts for four cities:
Paris, New York, Helsinki, and Tampere. It operates in either English or French, with English set as the default language.

## User Interactions

Users can inquire about the humidity level, weather, forecast (which includes weather and temperature), and temperature (in either Fahrenheit or Celsius). 
The system can parse the following types of questions:

- "What is the weather in Paris today ?" (Question mark is optional)
- "How is the weather in Helsinki right now ?"
- "What will be the temperature in New York next week ?" (Celsius by default)
- "What was the temperature in Tampere yesterday in Fahrenheit ?" (Can be asked in Celsius)
- "What is the humidity level in Paris right now ?"
- "What was the weather forecast for Helsinki yesterday ?" (This will provide both weather and temperature in Celsius)

## Error Handling and Keyword Recognition

The system is designed to handle parsing errors, such as when a question mark is not properly spaced. 
For example, 'What is the weather in Paris today?"' will still return the correct output from the dictionary.

The system also recognizes keywords. If a user types only a city name, a weather condition (humidity, weather, forecast, temperature), and a time, 
the system will still provide an output. 
For example, typing 'Paris humidity today' will return: "The humidity level in Paris today is 75 percent."

If the system cannot find data related to a city, it will print an 'unknown data command' message.
**Data for Helsinki next week is not present in the dictionary.

***To exit the program, simply type exit.

## Note
Initially, the system was designed to default to the time 'right now' if no specific time was mentioned. 
However, this feature was removed as it led to inaccuracies when parsing errors occurred.
For instance, if a user were to ask, "What is the temperature for Paris next month?" the system would print the temperature for 'right now'
as 'next month' is not present in both rules nor dictionary.


***Please note that the input text is case sensitive due to the grammar rules. Therefore, the first letter of city names must be capitalized.
Failure to do so will result in a parsing error and an 'invalid command' message.


*** The GF files require resource library: 'SymbolicEng'.
