

# Example Json Commands


## Lighting

{
  "AnimationMode":"Scene",
  "SceneName":"ColourSingle",
  "Brightness":100,
  "Transition":{"RateMs":1000,"TimeMs":1000,"Order":"InOrder"},
  "Scene":{"Colour":[55,2,55,0,0]},
}

{
  "AnimationMode":"Flasher",
  "Flasher":{"Function":1},
  "PixelGrouping":{"Enabled":1,"Mode":3},
  "Brightness":100
}

{
  "AnimationMode":"Scene",
  "SceneName":"ColourSingle",
  "Brightness":100,
  "BrightnessCCT":10,
  "colour_order":"RGB",
  "Transition":{"RateMs":1000,"TimeMs":1000,"Order":"InOrder"},
  "Scene":{"Colour":[255,150,0,0,255]},
}

{
  "Effects": {
    "Current": 51,
    "Speed": 128,
    "Intensity": 128,
    "Palette": 12
  }
}

{
  "colour_order":"GRB",
  "ColourPalette":10,
  "PaletteEdit": {
    "ColourPalette": 10,
    "Data": [1,2,3,4,5]
  },
  "AnimationMode":"Effects",
  "Effects":{"Function":1},
  "BrightnesRGB":100,
  "Transition":{"RateMs":1000,"TimeMs":1000,"Order":"InOrder"}
}




{
  "colour_order":"GRB",
  "ColourPalette":10,
  "Brightness":100,
  "PaletteEdit": {
    "ColourPalette": 10,
    "Data": [25,2,0,5,5]
  },
  "AnimationMode":"Effects",
  "Effects":{"Function":8},
  "BrightnessRGB":100,
  "BrightnessCCT":10,
  "Transition":{"RateMs":1000,"TimeMs":980,"Order":"Random"}
}


{
  "ColourPalette":33,
  "AnimationMode":"Effects",
  "Effects":{"Function":1},
  "BrightnessRGB":100,
  "Transition":{"RateMs":1000,"TimeMs":980,"Order":"Random"}
}

{
  "Notifications": {
    "PixelNum": [0,1,2,3],
    "Mode":[1,3,4,5],
    "Brightness":10,
    "TimeSecs":[8,10],
    "Colour":[
      [255,0,0],[0,255,0],[0,0,255],[255,0,255]
    ]
  }
}


