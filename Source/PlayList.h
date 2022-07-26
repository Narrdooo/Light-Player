#pragma once
#include <JuceHeader.h>
#include "PlayerPanel.h"

class PlayList :    public Component,
                    public TableListBoxModel
{
public:
    PlayList(PlayerPanel* pp);

    int getNumRows() override;
    void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected) override;
    void sortOrderChanged(int newSortColumnId, bool isForwards) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/,
                                       Component* existingComponentToUpdate) override;
    int getColumnAutoSizeWidth(int columnId) override;

    int getSelection(const int rowNumber) const;
    void setSelection(const int rowNumber, const int newSelection);

    String getText(const int columnNumber, const int rowNumber) const;
    void setText(const int columnNumber, const int rowNumber, const String& newText);

    void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent&) override;
    void resized() override;

private:
    TableListBox table{ {}, this };
    Font font{ 14.0f };

    std::unique_ptr<XmlElement> tutorialData;
    XmlElement* columnList = nullptr;
    XmlElement* dataList = nullptr;
    int numRows = 0;

    class EditableTextCustomComponent;
    class SelectionColumnCustomComponent;
    class TutorialDataSorter;

    std::unique_ptr<PlayerPanel> owner;

    void loadData(File tableFile);
    void readFromFolder(File folder);

    String getAttributeNameForColumnId(const int columnId) const;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayList)
};
