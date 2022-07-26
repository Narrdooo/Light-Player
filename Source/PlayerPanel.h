#pragma once

#include <JuceHeader.h>
#include "AudioUtils.h"
#include "AudioPlayer.h"


class PlayerPanel : public Component,
                    public FileDragAndDropTarget
{
public:
    //==============================================================================
    PlayerPanel();
    ~PlayerPanel() override;

    //==============================================================================
    void paint (Graphics&) override;
    void showList();

    bool isInterestedInFileDrag(const StringArray& files) override;
    void fileDragEnter(const StringArray& files, int, int) override;
    void fileDragMove(const StringArray& files, int, int) override;
    void fileDragExit(const StringArray& files) override;
    void filesDropped(const StringArray& files, int, int) override;

    class PlayList;

    struct InstrumentPanel : public Component
    {
        InstrumentPanel();
        ~InstrumentPanel();
        
        OwnedArray<ImageButton> buttons;
        OwnedArray<Image> icons;
        
        int vericalSpace = 20;
        int horizontalSpace = 30;
        int iconSize = 50;
        int iconBoarderSize = 70;
        int iconShadowSize = 2;
        int iconInterval = 20;
        int cornerSize = 20;
        int iconLabelSize = 20;

        int width = 530;    // iconInterval * 6 + iconBoarderSize * 5
        int height = 300;   // iconInterval * 3 + iconBoarderSize * 2 + iconLabelSize * 2

    };

    struct LeftPanel : public Component
    {
    public:
        LeftPanel();
        ~LeftPanel();

        InstrumentPanel instrumentPanel;
        AudioPlayer audioPlayer;
        int headerSize = 60;
    };


    LeftPanel leftPanel;
    std::unique_ptr<PlayList> playList;
    std::unique_ptr<AudioUtils> audioUtils;
    ImageButton hideButton;
    ImageButton closeButton;

        
    int shadowSize = 0;
    int cornerSize = 30;

private:
    //==============================================================================
    struct SelectedBox : public Component
    {
    public:
        SelectedBox();
        ~SelectedBox();
        void initial(int s, int c);
        void paint(Graphics& g) override;

        int size;
        int cornerSize;
    };

    OwnedArray<String> instrumentNames;
    SelectedBox selectedBox;

    void moveSelectedBox(int x, int y);
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerPanel)
};



class PlayerPanel::PlayList :   public Component,
                                public TableListBoxModel
{
public:
    PlayList(PlayerPanel& pp);

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

    int width = 240;
    Colour backgroundColour = Colour::fromRGB(58, 78, 124);
    //Colour fontColour = Colours::grey;
    Colour lineColour = Colour::fromRGB(47, 63, 99);

private:
    TableListBox table{ {}, this };
    Font font{ 12.0f };

    std::unique_ptr<XmlElement> tutorialData;
    XmlElement* columnList = nullptr;
    XmlElement* dataList = nullptr;
    int numRows = 0;
    int rowWidth = 0;
    int rowHeight = 60;

    class TutorialDataSorter;
    Image buttonImage = ImageCache::getFromMemory(BinaryData::rerun_png, BinaryData::rerun_pngSize);

    //struct SongInfo : public Component
    //{
    //public:
    //    SongInfo(PlayList& td)
    //        : owner(td)
    //    {
    //        addAndMakeVisible(button);

    //        //button.onClick = [this] { owner.setSelection(row, (int)button.getToggleState()); };
    //    }

    //    void resized() override
    //    {
    //        button.setBoundsInset(BorderSize<int>(2));
    //    }

    //    void setRowAndColumn(int newRow, int newColumn)
    //    {
    //        row = newRow;
    //        columnId = newColumn;

    //        button.setBounds(0, 20, buttonSize, buttonSize);
    //        button.setImages(false, true, false,
    //                         buttonImage, 1.0f, Colours::transparentBlack,
    //                         buttonImage, 0.7f, Colours::transparentBlack,
    //                         buttonImage, 0.2f, Colours::transparentBlack);
    //    }

    //private:
    //    PlayList& owner;
    //    ImageButton button;
    //    Image buttonImage = ImageCache::getFromMemory(BinaryData::rerun_png, BinaryData::rerun_pngSize);
    //    int row, columnId;
    //    int buttonSize = 20;
    //};

    PlayerPanel& owner;
    void loadData(File tableFile);
    void readFromFolder(File folder);
    void refreshState(XmlElement* data);
    void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;

    String getAttributeNameForColumnId(const int columnId) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayList)
};