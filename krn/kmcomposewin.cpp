// kmcomposewin.cpp

#include <klocale.h>
#include <unistd.h>
#include <qfiledlg.h>
#include <qaccel.h>
#include <qlabel.h>
#include "kmcomposewin.moc"
#include "kmmessage.h"
#include "kmsender.h"
#include <iostream.h>
#include <qwidget.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mimelib/string.h>
#include <mimelib/utility.h>
#include <kiconloader.h>
#include "groupdlg.h"
#include "kmidentity.h"

extern KLocale *nls;
extern KMSender *msgSender;
extern Groupdlg *main_widget;
extern KMIdentity *identity;

//-----------------------------------------------------------------------------
KMComposeView::KMComposeView(QWidget *parent, const char *name, 
                             QString emailAddress, KMMessage *message,
                             Action action,bool isNNTP,QString groups,bool isMail):
QWidget(parent, name)
{
    futureMessage=new KMMessage();
    
    QLabel* label;
    QSize sz;
    
    attWidget = NULL;
    if (message) currentMessage = message;
    else currentMessage = new KMMessage(); 
    
    grid = new QGridLayout(this,11,2,2,4);
    
    toLEdit = new QLineEdit(this);
    label = new QLabel(toLEdit, nls->translate("&To:"), this);
    label->adjustSize();
    label->setMinimumSize(label->size());
    if (isMail)
        grid->addWidget(label,0,0);
    
    sz.setWidth(100);
    sz.setHeight(label->size().height() + 6);
    
    toLEdit->setMinimumSize(sz);
    if (isMail)
        grid->addWidget(toLEdit,0,1);
    
    if (!isMail)
    {
        label->hide();
        toLEdit->hide();
    }
    
    ccLEdit = new QLineEdit(this);
    label = new QLabel(ccLEdit, nls->translate("&Cc:"), this);
    label->adjustSize();
    label->setMinimumSize(label->size());
    if (isMail)
        grid->addWidget(label,1,0);
    
    if (!isMail)
    {
        label->hide();
        ccLEdit->hide();
    }
    
    
    subjLEdit = new QLineEdit(this);
    label = new QLabel(subjLEdit, nls->translate("&Subject:"), this);
    label->adjustSize();
    label->setMinimumSize(label->size());
    grid->addWidget(label,2,0);
    connect(subjLEdit,SIGNAL(textChanged(const char *)),
            SLOT(slotUpdateHeading(const char *)));
    
    if (emailAddress) 
        toLEdit->setText(emailAddress);
    ccLEdit->setMinimumSize(sz);
    
    if (isMail)
        grid->addWidget(ccLEdit,1,1);
    
    subjLEdit->setMinimumSize(sz);
    grid->addWidget(subjLEdit,2,1);
    
    //Start of stuff for group names

    groupsLEdit = new QLineEdit(this);
    label = new QLabel(groupsLEdit, "&Groups:", this);
    label->adjustSize();
    label->setMinimumSize(label->size());
        groupsLEdit->setMinimumSize(sz);

    if (isNNTP)
    {
        grid->addWidget(label,3,0);
        grid->addWidget(groupsLEdit,3,1);
        if (groups)
            groupsLEdit->setText(groups);
    }
    else
    {
        label->hide();
        groupsLEdit->hide();
    }
    //End of stuff for group names
    
    
    editor = new KEdit(0,this);
    grid->addMultiCellWidget(editor,4,9,0,1);
    grid->setRowStretch(4,100);
    
    // Setup attachmentListBox
    attachmentListBox = new KTabListBox(this,NULL,3);
    attachmentListBox->setColumn(0,nls->translate("F"),20,
                                 KTabListBox::PixmapColumn);
    attachmentListBox->setColumn(1,nls->translate("Filename"),
                                 parent->width()-140);
    attachmentListBox->setColumn(2,nls->translate("Size"),80);
    connect(attachmentListBox,SIGNAL(popupMenu(int,int)),
            SLOT(slotPopupMenu(int,int)));
    grid->addMultiCellWidget(attachmentListBox,10,10,0,1);
    attachmentListBox->hide(); //Hide because no attachments present at startup  
    
    zone = new KDNDDropZone(editor,DndURL);
    connect(zone,SIGNAL(dropAction(KDNDDropZone *)),
            SLOT(slotGetDNDObject()));
    
    // urlList is the internal QStrList which holds 
    // all bodyParts to be displayed
    urlList = new QStrList; 
    
    grid->setColStretch(1,100);
    
    // Check if an action is requested.
    if(message && action==actForward) forwardMessage();	
    else if(message && action==actReply) replyMessage();
    else if(message && action ==actReplyAll) replyAll();
    else if(message && action ==actFollowup) followupMessage();
    
    grid->activate();
    
    parseConfiguration();	
    initKMimeMagic(); // Necessary for content Type parsing.
}

void KMComposeView::initKMimeMagic()
{
    // Magic file detection init
    QString mimefile = kapp->kdedir();
    mimefile += "/share/mimelnk/magic";
    magic = new KMimeMagic( mimefile );
    magic->setFollowLinks( TRUE );
}


//-----------------------------------------------------------------------------
KMComposeView::~KMComposeView()
{}

//-----------------------------------------------------------------------------
void KMComposeView::slotPrintIt()
{
    // QPrinter is extremly broken. Even the Trolls admitted
    // that. They said they would fix it in version 1.3.
    // For now printing is crap.
    
    QPrinter *printer = new QPrinter();
    if ( printer->setup(this) ) {
        QPainter paint;
        paint.begin( printer );
        QString text;
        text += nls->translate("To:");
        text += " \n";
        text += toLEdit->text();
        text += "\n";
        text += nls->translate("Subject:");
        text += " \n";
        text += subjLEdit->text();
        text += nls->translate("Date:");
        text += " \n\n";
        text += editor->text();
        text.replace(QRegExp("\n"),"\n");
        paint.drawText(30,30,text);
        paint.end();
    }
    delete printer;
}

//-----------------------------------------------------------------------------
void KMComposeView::slotAttachFile()
{
    QString fileName;
    
    // Create File Dialog and return selected file
    // We will not care about any permissions, existence or whatsoever in 
    // this function.
    QFileDialog *d = new QFileDialog(".","*",this,NULL,TRUE);
    d->setCaption(nls->translate("Attach File"));
    if (d->exec()) 
        fileName = d->selectedFile();		
    delete d;
    
    if(fileName.isEmpty()) // If cancel pressed return;
        return;
    urlList->append(fileName);
    
    if(urlList->count() == 1) // If first element in the aLB
    {createAttachmentWidget(); // Create attachmentListBox
        insertNewAttachment(fileName);} // and insert String into ListBox.
    else                     
        insertNewAttachment(fileName); // Else just insert String into ListBox
}

//-----------------------------------------------------------------------------
void KMComposeView::slotGetDNDObject()
{
    QString element;
    QStrList *tempList;
    
    tempList = new QStrList(); 
    *tempList= zone->getURLList();
    element = tempList->first();
    
    if(element.find("file:",0,0) >= 0)
        element.replace("file:","");
    
    urlList->append(element); // Insert DNDObject into internal List
    
    /* The following is only debug information
     element = urlList->first();
     cout << "Elements in the list: " << urlList->count() << "\n";
     while(element.isEmpty() == FALSE)
     {cout << element << "\n";
     element = urlList->next();}
     */
    
    if(urlList->count() == 1) // If DNDObject was first element in internal List
    {createAttachmentWidget(); // create new attachmentListBox 
        insertNewAttachment(tempList->first());} // and insert the element
    else
        insertNewAttachment(tempList->first()); // else just insert the element
    delete tempList;       
}

//-------------------------------------------------------------------------
void KMComposeView::createAttachmentWidget()
{
    // Obvious what this function does.
    attachmentListBox->show();
    grid->setRowStretch(4,3);
    grid->setRowStretch(10,1);
    
}


//--------------------------------------------------------------------------

void KMComposeView::insertNewAttachment(QString File)
{
    QString element;
    QString temp;
    struct stat filestat;
    
    cout << "Inserting Attachment\"" + File << "\" into widget\n";
    cout << "Elements to be inserted: " << urlList->count() << "\n";
    
    element = urlList->first();
    attachmentListBox->clear();
    
    while(element.isEmpty() == FALSE)
    {cout << element << "\n";
        stat(element.data(),&filestat); // Get file stats.
    
    if(filestat.st_size > 1024) // Calculate the size
        temp.sprintf("%.01f kb",(double)filestat.st_size/1024 * 1,0);
    else
        temp.sprintf("%ld bytes",filestat.st_size);
    
    element = " \n" +  element +  " \n " + temp; // The page.xpm file is still missing.
    attachmentListBox->insertItem(element);
    element = urlList->next();
    }
    
    resize(this->size());
    
}
//----------------------------------------------------------------------------
void KMComposeView::slotOpenAttachment()
{
    // This function will open a bodyPart that is visible 
    // in the attachmentListBox
    slotToDo();
}


//-----------------------------------------------------------------------------
void KMComposeView::slotRemoveAttachment()
{
    int index;
    
    index = attachmentListBox->currentItem();
    attachmentListBox->removeItem(index); // Remove item from TabListBox
    urlList->remove(index); // Remove item from internal list
    
    if(attachmentListBox->count() == 0) // Hide ListBox if it was 
    {attachmentListBox->hide(); // the last item in the list
        grid->setRowStretch(4,100);
    grid->setRowStretch(10,0);
    resize(this->size());
    }
}

//-----------------------------------------------------------------------------
void KMComposeView::slotShowProperties()
{
    // This function will show bodyPart Properties.
    slotToDo();
}

//-----------------------------------------------------------------------------
KMMessage * KMComposeView::prepareMessage()
{
    // This function is -the- function. It is called before sending the message.
    // Called from slotsendNow() and slotSendLater().
    // 1. It sets the necessary Message stuff.
    // 2a. If it is a simple text mail it sets msg's body.
    // 2b. It there are bodyParts to be added it creates the bodyParts
    // and adds them to msg.
    // 3. setAutomaticFields is called.
    
    QString option;
    QString temp,temp2;
    KMMessage *msg;
    
    msg = new KMMessage();
    msg = currentMessage; //msg 
    
    temp=toLEdit->text();
    temp2=groupsLEdit->text();
    if (temp.isEmpty() && temp2.isEmpty())
    {
        warning(nls->translate("No recipients or groups defined."));
        msg = 0;
        return msg;
    }
    
    temp = editor->text(); // temp is now the editor's text.
    
    // The the necessary Message() stuff
    
    msg->setFrom(EMailAddress);
    msg->setReplyTo(ReplyToAddress);
    msg->setTo(toLEdit->text());
    msg->setCc(ccLEdit->text());
    msg->setSubject(subjLEdit->text());
    msg->setGroups(groupsLEdit->text());
    msg->setReferences(futureMessage->references());
    
    if(urlList->count() == 0)// If there are no elements in the list waiting it is
        msg->setBody(temp); // a simple text message.
    else 
    {//	create bodyPart for editor text.
        KMMessagePart *part = new KMMessagePart();
        part->setCteStr("7-bit"); 
        part->setTypeStr("Text");
        part->setSubtypeStr("Plain");
        part->setBody(temp);
        msg->addBodyPart(part);
        
        // Since there is at least one more attachment create another bodypart
        QString atmntStr;
        atmntStr = urlList->first();
        part = new KMMessagePart();
        if((part = createKMMsgPart(part, atmntStr)) !=0)
            msg->addBodyPart(part);
        
        // As long as there are more attachments in the queue let's add bodyParts
        while((atmntStr = urlList->next()) != 0)
        {part = new KMMessagePart;
            if((part = createKMMsgPart(part,atmntStr)) != 0)
            {msg->addBodyPart(part);
                part = new KMMessagePart;
            }
            else
            {printf("no msgPart\n"); // Probably could not open file
                part = new KMMessagePart;
            }
        }
    }
    
    msg->setAutomaticFields();
    return msg;
}

KMMessagePart * KMComposeView::createKMMsgPart(KMMessagePart *p, QString s)
{
    printf("Creating MessagePart\n");
    DwString DwSrc;
    DwString DwDest;
    int pos;
    QFile *file = new QFile(s);
    QString str;
    QString temp;
    QString type;
    QString subType;
    char buf[255];
    
    p->setCteStr(((KMComposeWin*)parentWidget())->encoding); // Set Encoding
    
    if(!file->open(IO_ReadOnly))
    {KMsgBox::message(0,"Error","Can't open attachment file");
        p=0;
    return p;} // If we cannot open file set 'p = 0' and return p.
    // parseMessage then checks for the return value and if p=0 it performs
    // no action for that bodyPart
    
    file->at(0);
    while(file->readLine(buf,255) != 0)
        str.append(buf);
    file->close();
    
    KMimeMagicResult *result = new KMimeMagicResult();
    result = magic->findBufferType(str,str.length()-1);	
    temp =  result->getContent(); // Determine Content Type
    
    pos = temp.find("/",0,0); //Parse Content
    type = temp.copy();
    subType = temp.copy();
    type.truncate(pos);
    subType = subType.remove(0,pos+1); 
    cout << "Type:" << type << endl << "SubType:" << subType <<endl;
    
    // Set Content Type
    p->setTypeStr(type);
    p->setSubtypeStr(subType);
    
    // Encode data and set ContentTransferEncoding
    if(((KMComposeWin *)parentWidget())->encoding.find("base64",0,0) > -1)
    {debug("found base64\n");
        DwSrc.append(str);
    DwEncodeBase64(DwSrc,DwDest);
    str = DwDest.c_str();
    p->setCteStr("base64");
    }
    else
    {debug("encoding qtp\n");
        DwSrc.append(str);
    DwEncodeQuotedPrintable(DwSrc,DwDest);
    str = DwDest.c_str();
    p->setCteStr("quoted-printable");
    }
    
    p->setBody(str); // Set body.
    printf("Leaving MessagePart....\n");
    return p;
    
}



//----------------------------------------------------------------------------

void KMComposeView::slotSendNow()
{
    bool success=true;
    KMMessage *msg = new KMMessage();
    if((msg = prepareMessage()) == 0)
        return;
    bool doit=false;
    if (!strlen(msg->groups()))
    {
        doit=true;
    }
    else if (main_widget->needsConnect())
    {
        doit =true;
    }
    if (doit)
    {
        KMSender *tsender=0;
        if (!msgSender)
            tsender=new KMSender((NNTP *)0);
        else
            tsender=msgSender;
        debug ("about to send");
        tsender->setSmtpHost(SMTPServer);
        if (strlen (msg->to())>0) //if this has to be sent by mail
            success=tsender->send(msg,1);
        if (strlen (msg->groups())>0) //if this has to be sent to a newsgroup
            success=success && tsender->sendNNTP(msg,1);
        if (success)
            ((KMComposeWin *)parentWidget())->close();
        if (tsender !=msgSender)
            delete tsender;
    }
}

//----------------------------------------------------------------------------
void KMComposeView::slotSendLater()
{
    KMMessage *msg = new KMMessage();
    if((msg =prepareMessage()) == 0)
        return;
    if(msgSender->send(msg,FALSE))
        ((KMComposeWin *)parentWidget())->close();
}




void KMComposeView::slotPopupMenu(int index, int)
{
    // This popups the QPopupMenu in the attachmentListBox.
    attachmentListBox->setCurrentItem(index);
    const QPoint *point = new QPoint(200,200);
    QPopupMenu *p = new QPopupMenu();
    p->insertItem("Open",this,SLOT(slotOpenAttachment()));
    p->insertItem("Delete",this,SLOT(slotRemoveAttachment()));
    p->insertItem("Properties",this,SLOT(slotShowProperties()));
    p->popup(*point); // Some mapping has to be done here
    // but if I only new which.
}
//-----------------------------------------------------------------------------

void KMComposeView::parseConfiguration()
{
    // Obvious
    KConfig *config = new KConfig();
    config = KApplication::getKApplication()->getConfig();
    config->setGroup("Settings");
    QString o = config->readEntry("Signature");
    if( !o.isEmpty() && o.find("auto",0,false) ==0)
        slotAppendSignature();
    
    config->setGroup("NNTP");
    SMTPServer = config->readEntry("SMTPServer");
    NNTPServer = config->readEntry("NNTPServer");
    cout << SMTPServer << "\n";
    cout << NNTPServer << "\n";
    
    EMailAddress = identity->emailAddr();
    cout << EMailAddress << "\n";
    ReplyToAddress = config->readEntry("Reply-To Address");
    cout << ReplyToAddress << "\n";
}

//-----------------------------------------------------------------------------
void KMComposeView::forwardMessage()
{
    QString temp, spc;
    temp.sprintf(nls->translate("Fwd: %s"),currentMessage->subject().data());
    subjLEdit->setText(temp);
    
    spc = " ";
    
    editor->append(QString("\n\n---------")+nls->translate("Forwarded message")
                   +"-----------");
    editor->append(nls->translate("Date:") + spc + currentMessage->dateStr());
    editor->append(nls->translate("From:") + spc + currentMessage->from());
    editor->append(nls->translate("To:") + spc + currentMessage->to());
    editor->append(nls->translate("Cc:") + spc + currentMessage->cc());
    editor->append(nls->translate("Subject:") + spc + currentMessage->subject());
    
    editor->append(temp);
    if ((currentMessage->numBodyParts()) == 0) 
        temp = currentMessage->body();
    else
    {KMMessagePart *p = new KMMessagePart();
        currentMessage->bodyPart(0,p);
    temp = p->body();
    delete p;}
    editor->append(temp);
    
    editor->update();
    editor->repaint();
    
}

//-----------------------------------------------------------------------------
void KMComposeView::replyAll()
{
    QString temp;
    int lines;
    temp.sprintf(nls->translate("Re: %s"),currentMessage->subject().data());
    toLEdit->setText(currentMessage->from());
    ccLEdit->setText(currentMessage->cc());
    subjLEdit->setText(temp);
    
    temp.sprintf(nls->translate("\nOn %s %s wrote:\n"), 
                 currentMessage->dateStr().data(), currentMessage->from().data());
    editor->append(temp);
    
    //If there are no bodyparts take body.
    if ((currentMessage->numBodyParts()) == 0) 
        temp = currentMessage->body();
    else // if there are more than 0 bodyparts take bodyPart 0's body.
    {KMMessagePart *p = new KMMessagePart();
        currentMessage->bodyPart(0,p);
    temp = p->body();
    delete p;}  
    
    editor->append(temp);
    
    lines = editor->numLines();
    for(int x=2;x < lines;x++)
        editor->insertAt("> ",x,0);
    
    editor->update();
    
    currentMessage = currentMessage->createReply();
}

//-----------------------------------------------------------------------------
void KMComposeView::replyMessage()
{
    QString temp;
    int lines;
    
    temp.sprintf(nls->translate("Re: %s"),currentMessage->subject().data());
    toLEdit->setText(currentMessage->from());
    subjLEdit->setText(temp);
    
    temp.sprintf(nls->translate("\nOn %s %s wrote:\n"), 
                 currentMessage->dateStr().data(), currentMessage->from().data());
    editor->append(temp);
    
    if ((currentMessage->numBodyParts()) == 0) 
        temp = currentMessage->body();
    else
    {KMMessagePart *p = new KMMessagePart();
        currentMessage->bodyPart(0,p);
    temp = p->body();
    delete p;}
    
    
    editor->append(temp);
    
    lines = editor->numLines();
    for(int x=2;x < lines;x++)
    {editor->insertAt("> ",x,0);
    }
    editor->update();
    currentMessage = currentMessage->createReply();
}
//-----------------------------------------------------------------------------
void KMComposeView::followupMessage()
{
    QString temp;
    int lines;
    
    //futureMessage inherits the references, plus the ID
    
    temp.sprintf("%s %s",currentMessage->references().data(),currentMessage->id().data());
    futureMessage->setReferences (temp);
    
    temp.sprintf(nls->translate("Re: %s"),currentMessage->subject().data());
    groupsLEdit->setText(currentMessage->followup());
    subjLEdit->setText(temp);
    
    temp.sprintf(nls->translate("\nOn %s %s wrote:\n"), 
                 currentMessage->dateStr().data(), currentMessage->from().data());
    editor->append(temp);
    
    if ((currentMessage->numBodyParts()) == 0) 
        temp = currentMessage->body();
    else
    {KMMessagePart *p = new KMMessagePart();
        currentMessage->bodyPart(0,p);
    temp = p->body();
    delete p;}
    
    
    editor->append(temp);
    
    lines = editor->numLines();
    for(int x=2;x < lines;x++)
    {editor->insertAt("> ",x,0);
    }
    editor->update();
    currentMessage = currentMessage->createReply();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotUndoEvent()
{
}

//-----------------------------------------------------------------------------
void KMComposeView::slotCopyText()
{
    editor->copyText();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotCutText()
{
    editor->cut();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotPasteText()
{
    editor->paste();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotMarkAll()
{
    editor->selectAll();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotFind()
{
    editor->Search();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotSelectFont()
{
    editor->selectFont();
}

//-----------------------------------------------------------------------------
void KMComposeView::slotInsertFile()
{
    QString iFileString;
    QString textString;
    QFile *iFileName;
    char buf[255];
    int col, line;
    QFileDialog *d=new QFileDialog(".","*",this,NULL,TRUE);
    d->setCaption("Insert File");
    if (d->exec()) 
    {iFileString = d->selectedFile();
        iFileName = new QFile(iFileString);
    iFileName->open(IO_ReadOnly);
    iFileName->at(0);
    while(iFileName->readLine(buf,254) != 0)
        textString.append(buf);			
    iFileName->close();
    editor->getCursorPosition(&line,&col);
    editor->insertAt(textString,line,col);
    editor->update();
    editor->repaint(); 
    }
}	

void KMComposeView::slotUpdateHeading(const char *text)
{
    // This slot sets the Caption when text is entered in the subject LineEdit. 
    ((KMComposeWin*)parentWidget())->setCaption(text);
}

//-----------------------------------------------------------------------------
void KMComposeView::slotToDo()
{
    // Obvious. Hopefully I can get rid of this function soon. ;-)
    warning(nls->translate("Sorry, but this feature\nis still missing"));
}


//-----------------------------------------------------------------------------
void KMComposeView::slotAppendSignature()
{
    // Appends Signature file's text to editor's text.
    KConfig *configFile = new KConfig();
    QString sigFile;
    QString text;
    char temp[255];
    int col; 
    int line;
    
    editor->getCursorPosition(&line,&col); // Get position where to append
    configFile = KApplication::getKApplication()->getConfig();
    configFile->setGroup("Identity");
    sigFile = configFile->readEntry("Signature File");
    QFile *contFile= new QFile(sigFile);
    if(!contFile->open(IO_ReadOnly))
    {KMsgBox::message(0,"Error","Could not open Signature file!");
        return;}
    while((contFile->readLine(temp,100)) != 0)
        text.append(temp);
    contFile->close();
    editor->insertAt(text,line,col);
    editor->update();
    editor->repaint();
    
}


//-----------------------------------------------------------------------------
void KMComposeView::slotNewComposer()
{
    // Opens a new composer Dialog
    KMComposeWin *newComposer = new KMComposeWin(NULL,NULL,NULL,NULL);
    newComposer->show();
    newComposer->resize(newComposer->size());
    
}

void KMComposeWin::slotEncodingChanged()
{
    // This slot is called if the Encoding in the menuBar was changed
    if(menu->isItemChecked(menu->idAt(0)))
    {menu->setItemChecked(menu->idAt(0),FALSE);
        menu->setItemChecked(menu->idAt(1),TRUE);
    encoding="Quoted Printable";}
    else
    {menu->setItemChecked(menu->idAt(0),TRUE);
        menu->setItemChecked(menu->idAt(1),FALSE);
    encoding="BASE64";}
    
}

void KMComposeView::resizeEvent(QResizeEvent *)
{
}
//-----------------------------------------------------------------------------
KMComposeWin::~KMComposeWin()
{
    setCaption(nls->translate("Composer"));
    // saveProperties (false); Kalle: no longer needed
}

//-----------------------------------------------------------------------------
KMComposeWin::KMComposeWin(QWidget *, const char *name, QString emailAddress,
                           KMMessage *message, Action action,bool isNNTP,QString groups,bool isMail) :
KTopLevelWidget(name)
{
    setCaption(nls->translate("Composer"));
    // readProperties (); Kalle: no longer needed
    
    parseConfiguration();
    
    composeView = new KMComposeView(this,NULL,emailAddress,message,action,isNNTP,groups,isMail);
    setView(composeView,FALSE);
    
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    
    if(toolBarStatus==false)
        enableToolBar(KToolBar::Hide);	
    if(encoding.find("BASE64",0,0) > -1)	
        menu->setItemChecked(menu->idAt(0),TRUE);
    else
        menu->setItemChecked(menu->idAt(1),TRUE);
    resize(480, 510);
}

//-----------------------------------------------------------------------------
void KMComposeWin::show()
{
    KTopLevelWidget::show();
//    resize(size());
}

//-----------------------------------------------------------------------------
void KMComposeWin::parseConfiguration()
{
    KConfig *config;
    QString o;
    
    config = KApplication::getKApplication()->getConfig();
    config->setGroup("Settings");
    o = config->readEntry("ShowToolBar");
    if((!o.isEmpty() && o.find("no",0,false)) == 0)
        toolBarStatus = false; 
    else
        toolBarStatus = true;
    
    
    o = config->readEntry("Signature");
    if((!o.isEmpty() && o.find("auto",0,false)) == 0)
        sigStatus = false;   
    else
        sigStatus = true;
    
    
    o = config->readEntry("Send button");
    if((!o.isEmpty() && o.find("later",0,false)) == 0)
        sendButton = false;
    else
        sendButton = true;
    
    
    encoding = config->readEntry("Encoding");
    if(encoding.isEmpty())
        encoding="BASE64";
}	


//-----------------------------------------------------------------------------
void KMComposeWin::setupMenuBar()
{
    menuBar = new KMenuBar(this);
    
    QPopupMenu *fmenu = new QPopupMenu();
    fmenu->insertItem(nls->translate("Send"),composeView,
                      SLOT(slotSendNow()), ALT+Key_X);
    fmenu->insertItem(nls->translate("Send &later"),composeView,
                      SLOT(slotSendLater()),ALT+Key_L);
    fmenu->insertSeparator();
    fmenu->insertItem(nls->translate("Address &Book..."),composeView,
                      SLOT(slotToDo()),ALT+Key_B);
    fmenu->insertItem(nls->translate("&Print..."),composeView,
                      SLOT(slotPrintIt()),ALT+Key_P);
    fmenu->insertSeparator();
    fmenu->insertItem(nls->translate("&Close"),this,
                      SLOT(abort()),CTRL+ALT+Key_C);
    menuBar->insertItem(nls->translate("File"),fmenu);
    
    
    QPopupMenu  *emenu = new QPopupMenu();
    emenu->insertItem(nls->translate("Undo"),composeView,
                      SLOT(slotUndoEvent()));
    emenu->insertSeparator();
    emenu->insertItem(nls->translate("Cut"),composeView,
                      SLOT(slotCutText()),CTRL + Key_X);
    emenu->insertItem(nls->translate("Copy"),composeView,
                      SLOT(slotCopyText()),CTRL + Key_C);
    emenu->insertItem(nls->translate("Paste"),composeView,
                      SLOT(slotPasteText()),CTRL + Key_V);
    emenu->insertItem(nls->translate("Mark all"),composeView,
                      SLOT(slotMarkAll()),CTRL + Key_A);
    emenu->insertSeparator();
    emenu->insertItem(nls->translate("Find..."),composeView,
                      SLOT(slotFind()));
    menuBar->insertItem(nls->translate("Edit"),emenu);
    
    QPopupMenu *mmenu = new QPopupMenu();
    mmenu->insertItem(nls->translate("Recip&ients..."),composeView,
                      SLOT(slotToDo()),ALT+Key_I);
    mmenu->insertItem(nls->translate("Insert &File"), composeView,
                      SLOT(slotInsertFile()),ALT+Key_F);
    mmenu->insertSeparator();
    
    QPopupMenu *menv = new QPopupMenu();
    menv->insertItem(nls->translate("High"));
    menv->insertItem(nls->translate("Normal"));
    menv->insertItem(nls->translate("Low"));
    mmenu->insertItem(nls->translate("Priority"),menv);
    menuBar->insertItem(nls->translate("Message"),mmenu);
    
    QPopupMenu *amenu = new QPopupMenu();
    amenu->insertItem(nls->translate("&File..."),composeView,
                      SLOT(slotAttachFile()),ALT+Key_F);
    amenu->insertItem(nls->translate("Si&gnature"),composeView,
                      SLOT(slotAppendSignature()),ALT+Key_G);
    
    menu = new QPopupMenu();
    menu->setCheckable(TRUE);
    menu->insertItem(nls->translate("Base 64"),this,
                     SLOT(slotEncodingChanged()));
    menu->insertItem(nls->translate("Quoted Printable"),this,
                     SLOT(slotEncodingChanged()));
    amenu->insertItem(nls->translate("Encoding"),menu);
    menuBar->insertItem(nls->translate("Attach"),amenu);
    
    QPopupMenu *omenu = new QPopupMenu();
    omenu->insertItem(nls->translate("Toggle T&oolbar"),this,
                      SLOT(toggleToolBar()),ALT+Key_O);
    omenu->insertItem(nls->translate("Change &Font"),composeView,
                      SLOT(slotSelectFont()),ALT+Key_F);
    omenu->setItemChecked(omenu->idAt(2),TRUE);
    menuBar->insertItem(nls->translate("Options"),omenu);
    menuBar->insertSeparator();
    
    QPopupMenu *hmenu = new QPopupMenu();
    hmenu->insertItem(nls->translate("Help"),this,SLOT(invokeHelp()),ALT + Key_H);
    hmenu->insertSeparator();
    hmenu->insertItem(nls->translate("About"),this,SLOT(about()));
    menuBar->insertItem(nls->translate("Help"),hmenu);
    
    setMenu(menuBar);
}

//-----------------------------------------------------------------------------
void KMComposeWin::setupToolBar()
{
    KIconLoader* loader = kapp->getIconLoader();
    
    toolBar = new KToolBar(this);
    
    toolBar->insertButton(loader->loadIcon("filenew.xpm"),0,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotNewComposer()),TRUE,"Compose new message");
    toolBar->insertSeparator();
    
    toolBar->insertButton(loader->loadIcon("send.xpm"),0,
                          SIGNAL(clicked()),this,
                          SLOT(send()),TRUE,"Send message");
    toolBar->insertSeparator();
    toolBar->insertButton(loader->loadIcon("reload.xpm"),2,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotCopyText()),TRUE,"Undo last change");
    toolBar->insertButton(loader->loadIcon("editcopy.xpm"),3,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotCopyText()),TRUE,"Copy selection");
    toolBar->insertButton(loader->loadIcon("editcut.xpm"),4,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotCutText()),TRUE,"Cut selection");
    toolBar->insertButton(loader->loadIcon("editpaste.xpm"),5,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotPasteText()),TRUE,"Paste selection");
    toolBar->insertSeparator();
    toolBar->insertButton(loader->loadIcon("thumb_up.xpm"),6,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotToDo()),TRUE,"Recipients");
    toolBar->insertButton(loader->loadIcon("openbook.xpm"),7,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotToDo()),TRUE,"Open addressbook");
    toolBar->insertButton(loader->loadIcon("attach.xpm"),8,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotAttachFile()),TRUE,"Attach file");
    toolBar->insertSeparator();
    toolBar->insertButton(loader->loadIcon("fileprint.xpm"),12,
                          SIGNAL(clicked()),composeView,
                          SLOT(slotPrintIt()),TRUE,"Print message");
    toolBar->insertButton(loader->loadIcon("help.xpm"),13,
                          SIGNAL(clicked()),this,
                          SLOT(invokeHelp()),TRUE,"Help");
    
    addToolBar(toolBar);
}

//-----------------------------------------------------------------------------
void KMComposeWin::setupStatusBar()
{
    statusBar = new KStatusBar(this);
    statusBar->insertItem("Status:",0);
    setStatusBar(statusBar);
}

//-----------------------------------------------------------------------------
void KMComposeWin::toggleToolBar()
{
    enableToolBar(KToolBar::Toggle);
    if(toolBarStatus==false)
        toolBarStatus=true;
    else
        toolBarStatus=false;
    
    repaint();
}


//-----------------------------------------------------------------------------
void KMComposeWin::abort()
{
    close();
}
void KMComposeWin::about()
{  
    KMsgBox::message(this,nls->translate("About"),
                     "KMail [V0.1] by\n\n"
                     "Stefan Taferner <taferner@alpin.or.at>,\n"
                     "Markus W�bben <markus.wuebben@kde.org>\n\n" 
                     "based on the work of:\n"
                     "Lynx <lynx@topaz.hknet.com>,\n"
                     "Stephan Meyer <Stephan.Meyer@pobox.com>,\n"
                     "and the above authors.\n"
                     "This program is covered by the GPL.",1);
}

//-----------------------------------------------------------------------------
void KMComposeWin::invokeHelp()
{
    
    KApplication::getKApplication()->invokeHTMLHelp("","");
}

void KMComposeWin::send()
{
    if(!sendButton)
        composeView->slotSendNow();
    else
        composeView->slotSendLater();
}

//-----------------------------------------------------------------------------
void KMComposeWin::toDo()
{
    warning(nls->translate("Sorry, but this feature\nis still missing"));
}


//-----------------------------------------------------------------------------
void KMComposeWin::closeEvent(QCloseEvent *e)
{
    KTopLevelWidget::closeEvent(e);
    KConfig *config = new KConfig();
    config = KApplication::getKApplication()->getConfig();
    config->setGroup("Settings");
    config->writeEntry("ShowToolBar", toolBarStatus ? "yes" : "no");
    config->writeEntry("Encoding",encoding);
    config->sync();
    delete this;
}
