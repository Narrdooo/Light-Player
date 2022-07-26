#include "PlayerPanel.h"

PlayerPanel::PlayerPanel()
{
    audioUtils.reset(new AudioUtils());
    playList.reset(new PlayList(*this));
    playList->setSize(playList->width, leftPanel.getHeight());

    addAndMakeVisible(leftPanel);
    addAndMakeVisible(*playList);
    
    addAndMakeVisible(selectedBox);
    selectedBox.initial(leftPanel.instrumentPanel.iconBoarderSize + 10, leftPanel.instrumentPanel.cornerSize + 5);
    selectedBox.setVisible(false);
    selectedBox.setOpaque(false);

    //instrumentNames.add(new String("piano"));
    //instrumentNames.add(new String("guitar"));
    //instrumentNames.add(new String("cello"));
    //instrumentNames.add(new String("violin"));
    //instrumentNames.add(new String("saxophone"));
    //instrumentNames.add(new String("accordion"));
    //instrumentNames.add(new String("trumpet"));
    //instrumentNames.add(new String("vocal"));
    //instrumentNames.add(new String("more"));
    //instrumentNames.add(new String("plus"));

    instrumentNames.add(new String(CharPointer_UTF8("\xe9\x92\xa2\xe7\x90\xb4")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe5\x90\x89\xe4\xbb\x96")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe5\xa4\xa7\xe6\x8f\x90\xe7\x90\xb4")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe5\xb0\x8f\xe6\x8f\x90\xe7\x90\xb4")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe8\x90\xa8\xe5\x85\x8b\xe6\x96\xaf")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe6\x9c\xa8\xe7\xae\xa1")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe5\xb0\x8f\xe5\x8f\xb7")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe4\xba\xba\xe5\xa3\xb0\xe5\x90\x88\xe5\x94\xb1")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe6\x9b\xb4\xe5\xa4\x9a")));
    instrumentNames.add(new String(CharPointer_UTF8("\xe5\xa4\x96\xe9\x83\xa8\xe5\x90\x88\xe5\xa5\x8f")));


    centreWithSize(shadowSize + leftPanel.getWidth() + playList->getWidth(),
                   leftPanel.getHeight());

    leftPanel.setTopLeftPosition(shadowSize, 0);
    playList->setBounds(shadowSize + leftPanel.getWidth(), 
                        0, 
                        playList->getWidth(), 
                        playList->getHeight() - cornerSize);


    for (int i = 0; i < leftPanel.instrumentPanel.buttons.size(); i++) {
        auto bound = leftPanel.instrumentPanel.buttons[i]->getBounds();
        int x = bound.getCentreX() - 1;
        int y = bound.getCentreY() + leftPanel.headerSize + 1;

        leftPanel.instrumentPanel.buttons[i]->onClick = [this, i, x, y] { 
            audioUtils->createPluginInstance(i); 
            moveSelectedBox(x, y); 
        };
    }
}

void PlayerPanel::moveSelectedBox(int x, int y)
{
    selectedBox.setCentrePosition(x, y);
    selectedBox.setVisible(true);
}


PlayerPanel::~PlayerPanel()
{
    audioUtils.reset(nullptr);
    playList.reset(nullptr);
}

void PlayerPanel::paint(Graphics& g)
{
    setVisible(true);
    setOpaque(false);

    int thickness = 2;
    int playListWidth = playList->getWidth();
    // ------------------------- draw shadow -------------------------
    g.setColour(Colours::lightgrey);

    g.fillRect(0,
               0,
               getWidth() - cornerSize + shadowSize,
               getHeight() - cornerSize + shadowSize);

    g.fillRoundedRectangle(0, 
                           getHeight() - 2 * cornerSize,
                           getWidth() - shadowSize,
                           2 * cornerSize,
                           cornerSize);


    g.drawRoundedRectangle(shadowSize,
                           getHeight() - 2 * cornerSize - shadowSize,
                           leftPanel.getWidth() + cornerSize,
                           2 * cornerSize,
                           cornerSize,
                           1);
    // ------------------------- draw palylist -------------------------
    g.setColour(playList->backgroundColour);


    g.fillRoundedRectangle(leftPanel.getWidth() + shadowSize - 2 * cornerSize,
                           getHeight() - 2 * cornerSize - shadowSize,
                           playList->getWidth() + 2 * cornerSize,
                           2 * cornerSize, 
                           cornerSize);

    // ------------------------- white roundrect -------------------------
    g.setColour(Colours::white);

    g.fillRoundedRectangle(shadowSize,
                           getHeight() - 2 * cornerSize - shadowSize,
                           leftPanel.getWidth(),
                           2 * cornerSize, 
                           cornerSize);

    g.setColour(Colours::lightgrey);

    
    // ------------------------- white rect -------------------------
    g.setColour(Colours::white);

    g.fillRect(shadowSize,
               0, 
               leftPanel.getWidth(),
               getHeight() - cornerSize - shadowSize);


    // ------------------------- left line -------------------------
    g.setColour(Colours::lightgrey);

    g.drawLine(0,
               0,
               0,
               getHeight() - cornerSize + 3,
               1);

    // ------------------------- blue rect -------------------------
    g.setColour(playList->backgroundColour);

    g.fillRect(leftPanel.getWidth(),
               0, 
               playList->getWidth() + 2 * cornerSize,
               getHeight() - cornerSize);


    // ------------------------- draw seperate line -------------------------
    g.setColour(Colours::lightgrey);

    g.drawLine(shadowSize, 
               getHeight() - leftPanel.audioPlayer.getHeight() - shadowSize, 
               leftPanel.getWidth(), 
               getHeight() - leftPanel.audioPlayer.getHeight() - shadowSize);



    // ------------------------- draw logo -------------------------
    auto logoImage = ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);
    
    g.drawImage(logoImage, 
                40, 20, int(logoImage.getWidth() / 3), int(logoImage.getHeight() / 3),
                0, 0, logoImage.getWidth(), logoImage.getHeight(), 
                false);




    // ------------------------- draw buttons -------------------------
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            g.setColour(Colours::lightgrey);

            int iconInterval = leftPanel.instrumentPanel.iconInterval;
            int iconBoarderSize = leftPanel.instrumentPanel.iconBoarderSize;
            int iconLabelSize = leftPanel.instrumentPanel.iconLabelSize;
            int headerSize = leftPanel.headerSize;
            int horizontalSpace = leftPanel.instrumentPanel.horizontalSpace;
          
            g.fillRoundedRectangle(horizontalSpace + shadowSize + iconInterval * (j + 1) + iconBoarderSize * j - 2,
                                   horizontalSpace + headerSize + iconInterval * (i + 1) + (iconBoarderSize + iconLabelSize) * i + 2,
                                   iconBoarderSize,
                                   iconBoarderSize,
                                   leftPanel.instrumentPanel.cornerSize);

            g.setColour(Colours::whitesmoke);

            g.fillRoundedRectangle(horizontalSpace + shadowSize + iconInterval * (j + 1) + iconBoarderSize * j,
                                   horizontalSpace + headerSize + iconInterval * (i + 1) + (iconBoarderSize + iconLabelSize) * i,
                                   iconBoarderSize,
                                   iconBoarderSize,
                                   leftPanel.instrumentPanel.cornerSize);

            g.setColour(Colours::grey);

            g.setFont(Font("Microsoft YaHei UI", 15., Font::bold));
            g.drawSingleLineText(*instrumentNames[i * 5 + j],
                                 horizontalSpace + shadowSize + iconInterval * (j + 1) + iconBoarderSize * j + int(iconBoarderSize / 2),
                                 horizontalSpace + headerSize + (iconInterval + iconBoarderSize + iconLabelSize) * (i + 1),
                                 Justification::horizontallyCentred);
        }
    }

    addAndMakeVisible(hideButton);
    auto hideImage = ImageCache::getFromMemory(BinaryData::hide_png, BinaryData::hide_pngSize).rescaled(20, 15, Graphics::highResamplingQuality);
    hideButton.setImages(false, false, false,
                         hideImage, 1.0f, Colours::transparentBlack,
                         hideImage, 0.7f, Colours::transparentBlack,
                         hideImage, 0.2f, Colours::transparentBlack);
    hideButton.setBounds(420, 25, 20, 20);

    addAndMakeVisible(closeButton);
    auto closeImage = ImageCache::getFromMemory(BinaryData::close_png, BinaryData::close_pngSize).rescaled(20, 20, Graphics::highResamplingQuality);
    closeButton.setImages(false, false, false,
                         closeImage, 1.0f, Colours::transparentBlack,
                         closeImage, 0.7f, Colours::transparentBlack,
                         closeImage, 0.2f, Colours::transparentBlack);
    closeButton.setBounds(460, 25, 20, 20);
}


void PlayerPanel::showList()
{

}


bool PlayerPanel::isInterestedInFileDrag (const StringArray&) { return true; }

void PlayerPanel::fileDragEnter (const StringArray&, int, int) {}

void PlayerPanel::fileDragMove (const StringArray&, int, int) {}

void PlayerPanel::fileDragExit (const StringArray&) {}

void PlayerPanel::filesDropped (const StringArray& files, int x, int y) 
{
    //File firstFile{ files[0] };
    //if (files.size() > 1) {
    //    throw ("A file one time please!");
    //}
    //if (firstFile.hasFileExtension(String(".vst3"))) {
    //    midiPlayer->loadDroppedPlugin(files);
    //}
    //if (firstFile.hasFileExtension(String(".dll"))) {

    //}
    //midiPlayer->readMidiFile("C:/Users/ronnnhui/Documents/projects/Light-Player/BinaryData/test.mid");
    //midiPlayer->saveAudio("C:/Users/ronnnhui/Documents/projects/Light-Player/BinaryData/test.wav");
}


PlayerPanel::InstrumentPanel::InstrumentPanel() 
{
    setOpaque(false);
    setSize(width, height);

    auto pianoImage = ImageCache::getFromMemory(BinaryData::piano_png, BinaryData::piano_pngSize);
    auto guitarImage = ImageCache::getFromMemory(BinaryData::guitar_png, BinaryData::guitar_pngSize);
    auto celloImage = ImageCache::getFromMemory(BinaryData::cello_png, BinaryData::cello_pngSize);
    auto violinImage = ImageCache::getFromMemory(BinaryData::violin_png, BinaryData::violin_pngSize);
    auto saxophoneImage = ImageCache::getFromMemory(BinaryData::saxophone_png, BinaryData::saxophone_pngSize);
    auto accordionImage = ImageCache::getFromMemory(BinaryData::woodwind_png, BinaryData::woodwind_pngSize);
    auto trumpetImage = ImageCache::getFromMemory(BinaryData::trumpet_png, BinaryData::trumpet_pngSize);
    auto singerImage = ImageCache::getFromMemory(BinaryData::singer_female_png, BinaryData::singer_female_pngSize);
    auto moreImage = ImageCache::getFromMemory(BinaryData::more_png, BinaryData::more_pngSize);
    auto plusImage = ImageCache::getFromMemory(BinaryData::plus_png, BinaryData::plus_pngSize);

    icons.add(&pianoImage);
    icons.add(&guitarImage);
    icons.add(&celloImage);
    icons.add(&violinImage);
    icons.add(&saxophoneImage);
    icons.add(&accordionImage);
    icons.add(&trumpetImage);
    icons.add(&singerImage);
    icons.add(&moreImage);
    icons.add(&plusImage);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            auto button = new ImageButton();
            addAndMakeVisible(button);
            Image icon;
            if (i < 1 || j < 3) {
                icon = icons[i * 5 + j]->rescaled(iconSize, iconSize, Graphics::highResamplingQuality);
            }
            else {
                icon = icons[i * 5 + j]->rescaled(int(iconSize / 2), int(iconSize / 2), Graphics::highResamplingQuality);
            }
            button->setImages(false, false, false,
                              icon, 1.0f, Colours::transparentBlack,
                              icon, 0.7f, Colours::transparentBlack,
                              icon, 0.2f, Colours::transparentBlack);

            button->setBounds(horizontalSpace + iconInterval * (j + 1) + iconBoarderSize * j,
                              horizontalSpace + iconInterval * (i + 1) + iconBoarderSize * i + iconLabelSize * i,
                              iconBoarderSize,
                              iconBoarderSize);
            buttons.add(button);
        }
    }
}
PlayerPanel::InstrumentPanel::~InstrumentPanel() {}


PlayerPanel::LeftPanel::LeftPanel() 
{
    jassert(instrumentPanel.width == audioPlayer.width);

    setSize(instrumentPanel.width, 
            headerSize + instrumentPanel.height + audioPlayer.height);
    
    setOpaque(false);

    addAndMakeVisible(instrumentPanel);
    instrumentPanel.setTopLeftPosition(0, headerSize);

    addAndMakeVisible(audioPlayer);
    audioPlayer.setTopLeftPosition(0, headerSize + instrumentPanel.height);
}

PlayerPanel::LeftPanel::~LeftPanel() {}


PlayerPanel::SelectedBox::SelectedBox() {}
PlayerPanel::SelectedBox::~SelectedBox() {}

void PlayerPanel::SelectedBox::initial(int s, int c){
    size = s - 7;
    cornerSize = c;
    setSize(s+10, s+10);
}

void PlayerPanel::SelectedBox::paint(Graphics& g)
{
    
    g.setColour(Colour::fromRGB(180, 199, 231));
    g.drawRoundedRectangle(8, 7, size, size, cornerSize, 5);
}

//==============================================================================

class PlayerPanel::PlayList::TutorialDataSorter
{
public:
    TutorialDataSorter(const String& attributeToSortBy, bool forwards)
        : attributeToSort(attributeToSortBy),
        direction(forwards ? 1 : -1)
    {}

    int compareElements(XmlElement* first, XmlElement* second) const
    {
        auto result = first->getStringAttribute(attributeToSort)
            .compareNatural(second->getStringAttribute(attributeToSort)); // [1]

        if (result == 0)
            result = first->getStringAttribute("ID")
            .compareNatural(second->getStringAttribute("ID"));            // [2]

        return direction * result;                                        // [3]
    }

private:
    String attributeToSort;
    int direction;
};


PlayerPanel::PlayList::PlayList(PlayerPanel& pp)
    :owner(pp)
{

    addAndMakeVisible(table);

    loadData(File("C:/Users/roonn/Documents/projects/Light-Player/BinaryData/playlist.xml"));
    readFromFolder(File("C:/Users/roonn/Documents/projects/Light-Player/Resource"));

    //table.setColour(ListBox::outlineColourId, Colours::grey);      // [2]
    //table.setOutlineThickness(0);

    if (columnList != nullptr)
    {
        for (auto* columnXml : columnList->getChildIterator())
        {
            table.getHeader().addColumn(columnXml->getStringAttribute("name"), // [2]
                                        columnXml->getIntAttribute("columnId"),
                                        columnXml->getIntAttribute("width"),
                                        10,
                                        400,
                                        TableHeaderComponent::defaultFlags);
            rowWidth += columnXml->getIntAttribute("width");
        }
    }

    table.setRowHeight(rowHeight);
    table.getHeader().setSortColumnId(1, true);                                // [3]
    table.setMultipleSelectionEnabled(true);                                   // [4]
    //table.setColour(0x1002800, backgroundColour);
    table.setColour(table.backgroundColourId, backgroundColour);
    table.setColour(table.outlineColourId, Colours::transparentWhite);
    table.getVerticalScrollBar().setColour(table.getVerticalScrollBar().thumbColourId, lineColour);
    table.getHeader().setVisible(false);
    //table.getModel().
    
    resized();
    setOpaque(false);
}


void PlayerPanel::PlayList::loadData(File tableFile)
{
    if (tableFile == File() || !tableFile.exists())
        return;

    tutorialData = XmlDocument::parse(tableFile);            // [3]

    dataList = tutorialData->getChildByName("DATA");
    columnList = tutorialData->getChildByName("HEADERS");          // [4]

    numRows = dataList->getNumChildElements();                      // [5]
}


void PlayerPanel::PlayList::readFromFolder(File folder)
{
    jassert(folder.exists());
    for (auto subFolder : folder.findChildFiles(1, false))
    {
        if (subFolder.isDirectory()) {
            auto newData = new XmlElement("Song");
            newData->setAttribute("Name", subFolder.getFileName());
            newData->setAttribute("Path", subFolder.getFullPathName());
            refreshState(newData);
            dataList->addChildElement(newData);
            numRows += 1;
        }
    }
}

void PlayerPanel::PlayList::refreshState(XmlElement* data)
{
    auto subFolder = data->getStringAttribute("Path");
    auto file = File(File(subFolder).getFullPathName()).getChildFile("output.wav");
    auto length = owner.leftPanel.audioPlayer.getSongLength(file);
    data->setAttribute("Length", length);
    if (length == "") {
        data->setAttribute("State", "red");
    }
    else {
        data->setAttribute("State", "green");
    }
}

void PlayerPanel::PlayList::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
    if (columnId == 3) {
        auto data = dataList->getChildElement(rowNumber);
        auto path = data->getStringAttribute("Path");
        owner.audioUtils->run(path);
    }
}


void PlayerPanel::PlayList::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent&)
{
    auto path = dataList->getChildElement(rowNumber)->getAttributeValue(1);
    //owner.leftPanel.instrumentPanel.label.setText(path, dontSendNotification);
    owner.leftPanel.audioPlayer.addSongtoPlay(File(path).getChildFile("output.wav"));
    owner.leftPanel.audioPlayer.playOrPause();
}

int PlayerPanel::PlayList::getNumRows()
{
    return numRows;
}

void PlayerPanel::PlayList::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    //auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
    //    .interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
    int cornerSize = 10;
    if (rowIsSelected) {
        g.setColour(lineColour);
        g.fillRoundedRectangle(0, 0, rowWidth, rowHeight, cornerSize);
    }
    g.setColour(lineColour);
    g.drawLine(cornerSize, 0, rowWidth - cornerSize, 0);
    g.drawLine(cornerSize, rowHeight, rowWidth - cornerSize, rowHeight);
}

void PlayerPanel::PlayList::paintCell(Graphics& g, int rowNumber, int columnId,
                         int width, int height, bool rowIsSelected)
{
   //g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));  // [5]
    g.setColour(getLookAndFeel().findColour(ListBox::textColourId));
    if (auto* rowElement = dataList->getChildElement(rowNumber))
    {
        if (columnId == 1) {
            auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
            g.setFont(Font("Microsoft YaHei UI", 16., Font::bold));
            g.drawText(text, 20, 15, width - 10, height, Justification::topLeft, true);                             // [6]
            auto length = rowElement->getStringAttribute("Length");
            if (length.isEmpty()) {
                length = "- - : - -";
            }
            g.setFont(font);
            g.setColour(Colours::lightgrey);
            g.drawText(length, 20, 35, width - 10, height, Justification::topLeft, true);
        }
        else if (columnId == 2) {
            auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
            if (text == "red") {
                auto redDotImage = ImageCache::getFromMemory(BinaryData::redDot_png, BinaryData::redDot_pngSize);
                g.drawImage(redDotImage,
                            0, 25, 10, 10,
                            0, 0, redDotImage.getWidth(), redDotImage.getHeight());
            }
            else {
                auto greenDotImage = ImageCache::getFromMemory(BinaryData::greenDot_png, BinaryData::greenDot_pngSize);
                g.drawImage(greenDotImage,
                            0, 25, 10, 10,
                            0, 0, greenDotImage.getWidth(), greenDotImage.getHeight());
            }
        }
        else {
            g.drawImage(buttonImage,
                        0, 20, 20, 20,
                        0, 0, buttonImage.getWidth(), buttonImage.getHeight());
        }
        
    }

    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
}

void PlayerPanel::PlayList::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    if (newSortColumnId != 0)
    {
        PlayList::TutorialDataSorter sorter(getAttributeNameForColumnId(newSortColumnId), isForwards);
        dataList->sortChildElements(sorter);

        table.updateContent();
    }
}


Component* PlayerPanel::PlayList::refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/,
                                             Component* existingComponentToUpdate)
{
    //if (columnId == 9)  // [8]
    //{
    //    auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);

    //    if (selectionBox == nullptr)
    //        selectionBox = new SelectionColumnCustomComponent(*this);

    //    selectionBox->setRowAndColumn(rowNumber, columnId);
    //    return selectionBox;
    //}

    //if (columnId == 8)  // [9]
    //{
    //    auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

    //    if (textLabel == nullptr)
    //        textLabel = new EditableTextCustomComponent(*this);

    //    textLabel->setRowAndColumn(rowNumber, columnId);
    //    return textLabel;
    //}
    if (columnId == 1 || columnId == 2) {
        auto data = dataList->getChildElement(rowNumber);
        refreshState(data);
    }

    //if (columnId == 3) {
    //    auto* songInfo = static_cast<SongInfo*> (existingComponentToUpdate);

    //    if (songInfo == nullptr)
    //        songInfo = new SongInfo(*this);

    //    songInfo->setRowAndColumn(rowNumber, columnId);
    //    return songInfo;
    //}

    jassert(existingComponentToUpdate == nullptr);
    return nullptr;     // [10]
}

int PlayerPanel::PlayList::getColumnAutoSizeWidth(int columnId)
{
    if (columnId == 9)
        return 50;

    int widest = 32;

    for (auto i = getNumRows(); --i >= 0;)
    {
        if (auto* rowElement = dataList->getChildElement(i))
        {
            auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));

            widest = jmax(widest, font.getStringWidth(text));
        }
    }

    return widest + 8;
}

int PlayerPanel::PlayList::getSelection(const int rowNumber) const
{
    return dataList->getChildElement(rowNumber)->getIntAttribute("Select");
}

void PlayerPanel::PlayList::setSelection(const int rowNumber, int newSelection)
{
    dataList->getChildElement(rowNumber)->setAttribute("Select", newSelection);
}

String PlayerPanel::PlayList::getText(const int columnNumber, int rowNumber) const
{
    return dataList->getChildElement(rowNumber)->getStringAttribute(getAttributeNameForColumnId(columnNumber));
}

void PlayerPanel::PlayList::setText(const int columnNumber, const int rowNumber, const String& newText)
{
    auto columnName = table.getHeader().getColumnName(columnNumber);
    dataList->getChildElement(rowNumber)->setAttribute(columnName, newText);
}

void PlayerPanel::PlayList::resized()
{
    table.setBoundsInset(BorderSize<int>(8));
}



String PlayerPanel::PlayList::getAttributeNameForColumnId(const int columnId) const
{
    for (auto* columnXml : columnList->getChildIterator())
    {
        if (columnXml->getIntAttribute("columnId") == columnId)
            return columnXml->getStringAttribute("name");
    }

    return {};

}
