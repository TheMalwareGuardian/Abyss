# ***🧩 Abyss Bootkit Configuration Utility***

This tool is used to generate an encrypted configuration for the Abyss UEFI bootkit. It defines bootkit behavior (payload paths, flags, network options, etc.) and outputs a config file that is consumed by the UEFI application during early boot.


---
---
---


## ***📦 Structure***

- Templates/: Default templates to start from.
- AbyssConfiguration.py: Entry point of the configurator.
- Functions/ and Utils/: Logic for generating, encrypting, and obfuscating the configuration.
- AbyssConfiguration.ini: Holds global settings for encryption and obfuscation parameters.
- requirements.txt: Python dependencies.


---
---
---


## ***⚙️ Setup Instructions***


### ***🪟 Windows***

```
python -m venv venvAbyss
.\venvAbyss\Scripts\activate
pip install -r requirements.txt
```

### ***🐧 Linux***

```
python3 -m venv venvAbyss
source venvAbyss/bin/activate
pip install -r requirements.txt
```


---
---
---


## ***🚀 Usage***

After activating the virtual environment, run:
```
python AbyssConfiguration.py
```
