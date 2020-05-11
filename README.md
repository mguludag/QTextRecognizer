# QTextRecognizer
A gui for tesseractOCR with some preprocessing image options with OpenCV for better recognize text from images

[Download latest version (for Windows (vc15_x86))](https://github.com/mguludag/QTextRecognizer/releases/download/1.0.3/qtextrecognizer_vc15_x86.7z) ![GitHub Releases](https://img.shields.io/github/downloads/mguludag/QTextRecognizer/latest/total)

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
