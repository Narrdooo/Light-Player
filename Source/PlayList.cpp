//#include "PlayList.h"
//
//class PlayList::EditableTextCustomComponent : public Label
//{
//public:
//    EditableTextCustomComponent(PlayList& td)
//        : owner(td)
//    {
//        setEditable(false, true, false);
//    }
//
//    void mouseDown(const MouseEvent& event) override
//    {
//        owner.table.selectRowsBasedOnModifierKeys(row, event.mods, false);
//
//        Label::mouseDown(event);
//    }
//
//    void textWasEdited() override
//    {
//        owner.setText(columnId, row, getText());
//    }
//
//    void setRowAndColumn(const int newRow, const int newColumn)
//    {
//        row = newRow;
//        columnId = newColumn;
//        setText(owner.getText(columnId, row), dontSendNotification);
//    }
//
//private:
//    PlayList& owner;
//    int row, columnId;
//    Colour textColour;
//};
//
////==============================================================================
//class PlayList::SelectionColumnCustomComponent : public Component
//{
//public:
//    SelectionColumnCustomComponent(PlayList& td)
//        : owner(td)
//    {
//        addAndMakeVisible(toggleButton);
//
//        toggleButton.onClick = [this] { owner.setSelection(row, (int)toggleButton.getToggleState()); };
//    }
//
//    void resized() override
//    {
//        toggleButton.setBoundsInset(BorderSize<int>(2));
//    }
//
//    void setRowAndColumn(int newRow, int newColumn)
//    {
//        row = newRow;
//        columnId = newColumn;
//        toggleButton.setToggleState((bool)owner.getSelection(row), dontSendNotification);
//    }
//
//private:
//    PlayList& owner;
//    ToggleButton toggleButton;
//    int row, columnId;
//};
//
////==============================================================================
//class PlayList::TutorialDataSorter
//{
//public:
//    TutorialDataSorter(const String& attributeToSortBy, bool forwards)
//        : attributeToSort(attributeToSortBy),
//        direction(forwards ? 1 : -1)
//    {}
//
//    int compareElements(XmlElement* first, XmlElement* second) const
//    {
//        auto result = first->getStringAttribute(attributeToSort)
//            .compareNatural(second->getStringAttribute(attributeToSort)); // [1]
//
//        if (result == 0)
//            result = first->getStringAttribute("ID")
//            .compareNatural(second->getStringAttribute("ID"));            // [2]
//
//        return direction * result;                                        // [3]
//    }
//
//private:
//    String attributeToSort;
//    int direction;
//};
//
//
//PlayList::PlayList(PlayerPanel* pp)
//{
//    owner.reset(pp);
//    
//    loadData(File("C:/Users/ronnnhui/Documents/projects/Light-Player/BinaryData/playlist.xml"));
//    readFromFolder(File("C:/Users/ronnnhui/Documents/projects/Light-Player/Resource"));
//    addAndMakeVisible(table);
//    table.setColour(ListBox::outlineColourId, Colours::grey);      // [2]
//    table.setOutlineThickness(1);
//
//
//
//    if (columnList != nullptr)
//    {
//        for (auto* columnXml : columnList->getChildIterator())
//        {
//            table.getHeader().addColumn(columnXml->getStringAttribute("name"), // [2]
//                                        columnXml->getIntAttribute("columnId"),
//                                        columnXml->getIntAttribute("width"),
//                                        50,
//                                        400,
//                                        TableHeaderComponent::defaultFlags);
//        }
//    }
//
//    table.getHeader().setSortColumnId(1, true);                                // [3]
//
//
//    table.setMultipleSelectionEnabled(true);                                   // [4]
//    resized();
//}
//
//
//
//void PlayList::loadData(File tableFile)
//{
//    if (tableFile == File() || !tableFile.exists())
//        return;
//
//    tutorialData = XmlDocument::parse(tableFile);            // [3]
//
//    dataList = tutorialData->getChildByName("DATA");
//    columnList = tutorialData->getChildByName("HEADERS");          // [4]
//
//    numRows = dataList->getNumChildElements();                      // [5]
//}
//
//void PlayList::readFromFolder(File folder)
//{
//    jassert(folder.exists());
//    for (auto subFolder : folder.findChildFiles(1, false))
//    {
//        if (subFolder.isDirectory()) {
//            auto newData = new XmlElement("Song");
//            newData->setAttribute("ID", numRows+1);
//            newData->setAttribute("Name", subFolder.getFileName());
//            newData->setAttribute("Path", subFolder.getFullPathName());
//            //newData->setAttribute("State", );
//            dataList->addChildElement(newData);
//            numRows += 1;
//        }
//    }
//}
//
//void PlayList::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent&)
//{
//    auto path = dataList->getAttributeValue(3);
//    owner.leftPanel.audioPlayer.addSongtoPlay(path + "/output.wav");
//}
//
//int PlayList::getNumRows()
//{
//    return numRows;
//}
//
//void PlayList::paintRowBackground  (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
//{
//    auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
//        .interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
//    if (rowIsSelected)
//        g.fillAll(Colours::lightblue);
//    else if (rowNumber % 2)
//        g.fillAll(alternateColour);
//}
//
//void PlayList::paintCell (Graphics& g, int rowNumber, int columnId,
//                          int width, int height, bool rowIsSelected)
//{
//    g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));  // [5]
//    g.setFont(font);
//
//    if (auto* rowElement = dataList->getChildElement(rowNumber))
//    {
//        auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
//
//        g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);                             // [6]
//    }
//
//    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
//    g.fillRect(width - 1, 0, 1, height);                                                                               // [7]
//}
//
//void PlayList::sortOrderChanged (int newSortColumnId, bool isForwards)
//{
//    if (newSortColumnId != 0)
//    {
//        PlayList::TutorialDataSorter sorter(getAttributeNameForColumnId(newSortColumnId), isForwards);
//        dataList->sortChildElements(sorter);
//
//        table.updateContent();
//    }
//}
//
//Component* PlayList::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
//                                              Component* existingComponentToUpdate)
//{
//    if (columnId == 9)  // [8]
//    {
//        auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);
//
//        if (selectionBox == nullptr)
//            selectionBox = new SelectionColumnCustomComponent(*this);
//
//        selectionBox->setRowAndColumn(rowNumber, columnId);
//        return selectionBox;
//    }
//
//    if (columnId == 8)  // [9]
//    {
//        auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);
//
//        if (textLabel == nullptr)
//            textLabel = new EditableTextCustomComponent(*this);
//
//        textLabel->setRowAndColumn(rowNumber, columnId);
//        return textLabel;
//    }
//
//    jassert(existingComponentToUpdate == nullptr);
//    return nullptr;     // [10]
//}
//
//int PlayList::getColumnAutoSizeWidth(int columnId)
//{
//    if (columnId == 9)
//        return 50;
//
//    int widest = 32;
//
//    for (auto i = getNumRows(); --i >= 0;)
//    {
//        if (auto* rowElement = dataList->getChildElement(i))
//        {
//            auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));
//
//            widest = jmax(widest, font.getStringWidth(text));
//        }
//    }
//
//    return widest + 8;
//}
//
//int PlayList::getSelection(const int rowNumber) const
//{
//    return dataList->getChildElement(rowNumber)->getIntAttribute("Select");
//}
//
//void PlayList::setSelection(const int rowNumber, int newSelection)
//{
//    dataList->getChildElement(rowNumber)->setAttribute("Select", newSelection);
//}
//
//String PlayList::getText(const int columnNumber, int rowNumber) const
//{
//    return dataList->getChildElement(rowNumber)->getStringAttribute(getAttributeNameForColumnId(columnNumber));
//}
//
//void PlayList::setText(const int columnNumber, const int rowNumber, const String& newText)
//{
//    auto columnName = table.getHeader().getColumnName(columnNumber);
//    dataList->getChildElement(rowNumber)->setAttribute(columnName, newText);
//}
//
//void PlayList::resized()
//{
//    table.setBoundsInset(BorderSize<int>(8));
//}
//
//
//
//String PlayList::getAttributeNameForColumnId(const int columnId) const
//{
//    for (auto* columnXml : columnList->getChildIterator())
//    {
//        if (columnXml->getIntAttribute("columnId") == columnId)
//            return columnXml->getStringAttribute("name");
//    }
//
//    return {};
//   
//}
