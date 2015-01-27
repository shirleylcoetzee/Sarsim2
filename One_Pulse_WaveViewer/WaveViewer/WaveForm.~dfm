object fWave: TfWave
  Left = 187
  Top = 111
  Width = 821
  Height = 503
  Caption = 'Wave Viewer v0.1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object iWave: TImage
    Left = 8
    Top = 144
    Width = 800
    Height = 300
    Cursor = crCross
    Stretch = True
    OnMouseDown = iWaveMouseDown
    OnMouseMove = iWaveMouseMove
    OnMouseUp = iWaveMouseUp
  end
  object mCon: TMemo
    Left = 272
    Top = 1
    Width = 537
    Height = 136
    Color = clMenu
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      'Wave Viewer v0.1'
      'Click File->Open to begin'
      'Then Select which waveform you would like to view'
      ''
      'Console')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object gbData: TGroupBox
    Left = 8
    Top = 0
    Width = 257
    Height = 137
    Caption = 'gbData'
    Enabled = False
    TabOrder = 1
    object lPos: TLabel
      Left = 8
      Top = 118
      Width = 77
      Height = 13
      Caption = 'Current (x,y) = (,)'
    end
    object rbReal: TRadioButton
      Left = 8
      Top = 24
      Width = 73
      Height = 17
      Caption = 'Real'
      TabOrder = 0
      OnClick = rbRealClick
    end
    object rbImag: TRadioButton
      Left = 8
      Top = 40
      Width = 73
      Height = 17
      Caption = 'Imaginary'
      TabOrder = 1
      OnClick = rbImagClick
    end
    object rbMag: TRadioButton
      Left = 8
      Top = 56
      Width = 73
      Height = 17
      Caption = 'Magnitude'
      TabOrder = 2
      OnClick = rbMagClick
    end
    object rbPhase: TRadioButton
      Left = 8
      Top = 72
      Width = 73
      Height = 17
      Caption = 'Phase'
      TabOrder = 3
      OnClick = rbPhaseClick
    end
    object bSaveWave: TButton
      Left = 112
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Save JPEG'
      TabOrder = 4
      OnClick = bSaveWaveClick
    end
    object eImgFilename: TEdit
      Left = 112
      Top = 40
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'output1.jpg'
    end
  end
  object MainMenu1: TMainMenu
    Left = 32
    Top = 408
    object File1: TMenuItem
      Caption = 'File'
      object Open1: TMenuItem
        Caption = 'Open'
        OnClick = Open1Click
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
  end
  object odWave: TOpenDialog
    Filter = 'Sarsim2 ASC (*.asc)|*.asc'
    Top = 408
  end
end
