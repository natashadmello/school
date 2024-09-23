import pgf

# Read the pgf file and get the language out
gr = pgf.readPGF("WeatherBot.pgf")
eng = gr.languages["WeatherBotEng"]
fre = gr.languages["WeatherBotFre"]

# Initialize: Set default language
deflang = eng

def language_query():
    print("Choose a language: English or French")
    lang = "Unknown"
    while lang == "Unknown":
        lang_str = input().capitalize()  # Make the input case insensitive
        if lang_str == "English":
            lang = eng
        elif lang_str == "French":
            lang = fre
        else:
            print("Unknown language. Please choose English or French.")
    return lang



def chat(lang):

    # Choose language
    lang = language_query()
    
    
    # Define a dictionary to store weather information
    weather_data = {
    "Paris": {
        "today": ["clear sky", 10, 75, "is"],
        "right now": ["clear sky", 12, 80, "is"],
        "tomorrow": ["cloudy", 8, 70, "will be"],
        "day after": ["windy", 14, 85, "will be"],
        "yesterday": ["sunny", 11, 77, "was"],
        "next week": ["rainy", 9, 72, "will be"],
    },
    "New York": {
        "today": ["cloudy", 5, 87, "is"],
        "right now": ["thunderstorm", 7, 82, "is"],
        "tomorrow": ["cloudy", 10, 78, "will be"],
        "day after": ["windy", 8, 80, "will be"],
        "yesterday": ["sunny", 6, 85, "was"],
        "next week": ["rainy", 4, 88, "will be"],
    },
    "Tampere": {
        "today": ["snowy", -2, 75, "is"],
        "right now": ["clear sky", 0, 70, "is"],
        "tomorrow": ["sunny", 1, 80, "will be"],
        "day after": ["clear sky", -5, 67, "will be"],
        "yesterday": ["sunny", -3, 72, "was"],
        "next week": ["snowy", -1, 78, "will be"],
    },
    "Helsinki": {
        "today": ["clear sky", -4, 80, "is"],
        "right now": ["snowy", -8, 67, "is"],
        "tomorrow": ["sunny", 2, 75, "will be"],
        "day after": ["snowy", -10, 72, "will be"],
        "yesterday": ["sunny", 0, 50, "was"],
        #"next week": ["snowy", -9, 77, "will be"],
    },
    "Mumbai": {},
    }

    # Print welcome message
    welcome_e = pgf.readExpr("welcomeStart")
    print(lang.linearize(welcome_e))

    while True:
        # Get user input
        user_input = input()
        
        if user_input == 'exit':
            break

        # Parse the user input
        try:
            i = lang.parse(user_input)
            p, e = i.__next__()

            # Find city and time in the question
            label, components = e.unpack()
            #print(e.unpack())

            if label == 'QWeatherFact':
                city = components[1]
                time = components[2]
                variants = components[0]
                                
                weather_info = weather_data.get(lang.linearize(city), {}).get(lang.linearize(time))
                #print(weather_info)

                if weather_info is None:
                    # If we don't have the requested data, say unknown                   
                    response_expr = pgf.Expr("QUnknown", [pgf.readExpr("UnknownWeather"), city, time])
                    print(lang.linearize(response_expr))
                else:
                    # Otherwise, generate a response
                    wc = weather_info[0]
                    #print(wc)
                    wc_e = pgf.Expr(wc)
          
                    response_expr = pgf.Expr("AWeatherFact", [city, time, variants, wc_e])
                   
                    # Print or use the generated response
                    print(lang.linearize(response_expr))

            elif label == 'QTemperatureFact':
                city = components[1]
                time = components [2]
                temptype_e = components [3]
                variants = components [0]

                # Extract the value of temptype from the expression
                # temptype = temptype_e.unpack()[0]
                temptype = temptype_e.unpack()[0] if temptype_e.unpack() else None


                # Use the extracted information to look up the weather data
                temp_info = weather_data.get(lang.linearize(city), {}).get(lang.linearize(time))
                #print(temp_info)
                
                if temp_info is None:
                   response_expr = pgf.Expr("QUnknown",[pgf.readExpr("UnknownTemp"),city, time])
                   print(lang.linearize(response_expr))
                else:
                   temp = temp_info[1]
                   # Check the TemperatureType and convert to Fahrenheit if it's "Fah"
                   if temptype == 'Fah':
                      tempF = round((temp * 9/5) + 32)
                      temp_e = pgf.Expr(tempF)
                   else:
                      temp_e = pgf.Expr(temp)
                    
                   temptype_e2 = pgf.readExpr("Cel") if temptype is None else temptype_e
                   response_expr = pgf.Expr("ATemperatureFact", [city, time, variants, temp_e, temptype_e2])
                   print(lang.linearize(response_expr))

            elif label == 'QHumidityFact':
                city = components[1]
                time = components[2]
                variants = components [0]
                # Use the extracted information to look up the weather data
                humidity_info = weather_data.get(lang.linearize(city), {}).get(lang.linearize(time))
                # print(humidity_info)

                if humidity_info is None:
                  response_expr = pgf.Expr("QUnknown", [pgf.readExpr("UnknownHumid"), city, time])
                  print(lang.linearize(response_expr))
                else:
                  humidity = humidity_info[2]
                  humidity_e = pgf.Expr(humidity)
                  response_expr = pgf.Expr("AHumidityFact", [city, time, variants, humidity_e])
                  print(lang.linearize(response_expr))
            elif label == "QForecastFact":
                city = components[1]
                time = components[2]
                variants = components [0]

               # Use the extracted information to look up the weather data
                forecast_info = weather_data.get(lang.linearize(city), {}).get(lang.linearize(time))
                # print(forecast_info)

                if forecast_info is None:
                  response_expr = pgf.Expr("QUnknown", [pgf.readExpr("UnknownForecast"), city, time])
                  print(lang.linearize(response_expr))
                else:
                  wc = forecast_info[0]
                  temp = forecast_info[1]
                  
                  wc_e = pgf.Expr(wc)
                  temp_e = pgf.Expr(temp)
                  tt_e = pgf.readExpr("Cel")
        
                  response_expr = pgf.Expr("AForecastFact", [city, time, variants, wc_e, temp_e, tt_e])
                  print(lang.linearize(response_expr))


            else:
                print(lang.linearize(pgf.readExpr("qInvalid")))

        except pgf.ParseError:
            keywords = ['humidity', 'weather', 'forecast', 'temperature']
            if any(keyword in user_input for keyword in keywords):
            # Extract city and time from the user input
               user_input = user_input.replace('?', ' ')
               words = user_input.split()
               city = None
               time = None
               for i in range(len(words)):
                  # Check for two-word city names and time phrases
                  if i < len(words) - 1:
                     two_word_phrase = words[i] + ' ' + words[i+1]
                     if two_word_phrase in weather_data.keys():
                        city = two_word_phrase
                     elif two_word_phrase in ('right now', 'next week'):
                        time = two_word_phrase
                  # Check for one-word city names and time phrases
                  if words[i] in weather_data.keys():
                     city = words[i]
                  elif words[i] in ('today', 'tomorrow', 'yesterday'):
                     time = words[i]
               #print(city)
               #print(time)
               
               #if city is not None and time is None:
                  #time = 'right now'  ## if I ask question about next month, it will still print for right now, so unless introduced in dictionary, this wont work

               if city is None or time is None:
                   print(lang.linearize(pgf.readExpr("qInvalid")))
               else:
                   forecast_info = weather_data.get(city, {}).get(time)
                   city = pgf.Expr(city)
                   time = pgf.Expr(time)
                   if forecast_info is None:
                      response_expr = pgf.Expr("QUnknown", [pgf.readExpr("UnknownForecast"), city, time])
                      print(lang.linearize(response_expr))
                   else:
                      # Provide the corresponding fact based on the keyword
                      var = forecast_info[3]
                      variants = pgf.Expr(var)
                      if 'humidity' in user_input:
                          humidity = forecast_info[2]
                          humidity_e = pgf.Expr(humidity)
                          response_expr = pgf.Expr("AHumidityFact", [city, time, variants, humidity_e])
                          print(lang.linearize(response_expr))

                      if 'weather' in user_input:
                          wc = forecast_info[0]
                          #print(wc)
                          wc_e = pgf.Expr(wc)
                          response_expr = pgf.Expr("AWeatherFact", [city, time, variants, wc_e])
                          # Print or use the generated response
                          print(lang.linearize(response_expr))

                      if 'forecast' in user_input:
                          wc = forecast_info[0]
                          temp = forecast_info[1]
                          wc_e = pgf.Expr(wc)
                          temp_e = pgf.Expr(temp)
                          tt_e = pgf.readExpr("Cel")
                          response_expr = pgf.Expr("AForecastFact", [city, time, variants, wc_e, temp_e, tt_e])
                          print(lang.linearize(response_expr))

                      if 'temperature' in user_input:
                          temp = forecast_info[1]
                          if 'Fahrenheit' in user_input:
                             tempF = round((temp * 9/5) + 32)
                             temp_e = pgf.Expr(tempF)
                             temptype_e2 = pgf.readExpr("Fah")
                          else:
                             temp_e = pgf.Expr(temp)
                             temptype_e2 = pgf.readExpr("Cel")
                          response_expr = pgf.Expr("ATemperatureFact", [city, time, variants, temp_e, temptype_e2])
                          print(lang.linearize(response_expr))

         
            else:        
               print(lang.linearize(pgf.readExpr("qInvalid")))



chat(deflang)