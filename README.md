# QTextRecognizer
A gui for tesseractOCR with some preprocessing image options with OpenCV for better recognize text from images

![GitHub top language](https://img.shields.io/github/languages/top/mguludag/QTextRecognizer) ![GitHub All Releases](https://img.shields.io/github/downloads/mguludag/QTextRecognizer/total) ![GitHub Releases](https://img.shields.io/github/downloads/mguludag/QTextRecognizer/latest/total) ![GitHub release (latest by date)](https://img.shields.io/github/v/release/mguludag/QTextRecognizer) ![GitHub Release Date](https://img.shields.io/github/release-date/mguludag/QTextRecognizer)

<a href="https://github.com/mguludag/QTextRecognizer/releases/latest/download/setup.exe"><img alt="GitHub Releases" src="https://img.shields.io/github/downloads/mguludag/QTextRecognizer/latest/total?label=Download%20Latest%20Version%20(VC15%20x86)&style=for-the-badge">

#### Additional downloads

* You have to download languages from tesseract-ocr tessdata github page.

## How to use?

* Open the app, drag and drop picture or use open button.
* Apply the preprocess options you want:
  * Rotate
  * Remove warp
  * Threshold
  * Brightness, contrast
* Select the text blocks by mouse also press **SHIFT** key for select multiple text blocks.
* Press Recognize text button and adjust the options you want:
  * Select language(s)
  * Select Page segmentation mode
  * Insert black/white list for characters you want
  * Select adaptive threshold before recognition you want
  * Press Start OCR and wait for the result

## Screenshots

![Main screen](img/1.png)

![Rotate image screen](img/2.png)

![Deskew image screen](img/3.png)

![Image adjustments screen](img/4.png)

![Autodetect text layout](img/5.png)

![Recognize text screen](img/6.png)

![Settings screen and dark skin](img/7.png)



## Building

#### Dependencies

* Qt 5.x and Qt Creator
* OpenCV 4.x
* Tesseract 4.1
* Leptonica
