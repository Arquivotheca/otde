//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This file os part of KRN, a newsreader for the KDE project.              //
// KRN is distributed under the GNU General Public License.                 //
// Read the acompanying file COPYING for more info.                         //
//                                                                          //
// KRN wouldn't be possible without these libraries, whose authors have     //
// made free to use on non-commercial software:                             //
//                                                                          //
// MIME++ by Doug Sauder                                                    //
// Qt     by Troll Tech                                                     //
//                                                                          //
// This file is copyright 1997 by                                           //
// Roberto Alsina <ralsina@unl.edu.ar>                                      //
// Magnus Reftel  <d96reftl@dtek.chalmers.se>                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "NNTP.h"
#include <qstrlist.h>
#include <qlist.h>

extern ArticleDict artSpool;

extern QString krnpath,cachepath,artinfopath,groupinfopath;

////////////////////////////////////////////////////////////////////
// Article class. Represents an article
// Real docs soon.
////////////////////////////////////////////////////////////////////

Article::Article(void)
{
    setRead(false);
    setAvailable(true);
    setMarked (false);
    refcount=0;
    Refs.setAutoDelete(true);
    threadDepth=0;
}

void Article::decref()
{
    refcount--;
    if (!refcount)
    {
        artSpool.remove(ID.data());
    }
};

Article::~Article()
{
}


void Article::formHeader(QString *s)
// Builds a nice header to put in the header list, which properly
// Reflects the internal state of the message
{
    const char *s1, *s2, *s3;
    if (isRead())
    {
        s->setStr("R\n \n");
    }
    else
    {
        s->setStr("N\n \n");
    }
    if (!isAvailable())
    {
        s->setStr("T\n \n");
    }
    if (isMarked())
    {
        s->setStr("M\n \n");
    }

    char *tempbuf=new char[2048];
    if (From.data())
    {
        DwMailbox fromaddr;
        fromaddr.FromString (From.data());
        fromaddr.Parse();
        
        s1=fromaddr.FullName().c_str();
        s2=fromaddr.LocalPart().c_str();
        s3=fromaddr.Domain().c_str();
        sprintf (tempbuf,"%s <%s@%s>",s1,s2,s3);
        s->append(tempbuf);
    }
    else
    {
        s->append("Unkown Address");
    }
    s->append("\n \n");

    s->append(Lines);
    s->append("\n \n");

    QString t;
    t.setNum(score());
    s->append(t);
    s->append("\n \n");

    if (Date.data())
    {
        DwDateTime date;
        date.FromString(Date.data());
        date.Parse();
        sprintf(tempbuf,"%d/%d",date.Day(),date.Month());
        s->append(tempbuf);
    }
    else
    {
        s->append("0/0");
    }
    s->append("\n \n");
    delete[] tempbuf;

    for (int i=0;i<threadDepth;i++)
        s->append("  ");
    s->append(Subject);
}

void Article::save()
//stores the article info and data into the cache
{
    QString p=artinfopath+ID;
    QFile f(p.data());
    if(f.open (IO_WriteOnly))
    {
        QTextStream st(&f);
        st<<ID<<"\n";
        st<<Subject<<"\n";
        st<<Lines<<"\n";
        st<<From<<"\n";
        st<<ID<<"\n";
        st<<Date<<"\n";
        st<<isRead()<<"\n";
        st<<isAvailable()<<"\n";
        for (char *iter=Refs.first();iter!=0;iter=Refs.next())
        {
            st<<iter<<"\n";
        }
        f.close();
    }
}
void Article::load()
//gets the article info and data from the cache
{
    QString s;
    QString p=artinfopath+ID;
    QFile f(p.data());
    if(f.open (IO_ReadOnly))
    {
        QTextStream st(&f);
        ID=st.readLine();
        Subject=st.readLine();
        Lines=st.readLine();
        From=st.readLine();
        ID=st.readLine();
        Date=st.readLine();
        if (st.readLine()=="1")
            isread=true;
        else
            isread=false;
        if (st.readLine()=="1")
            isavail=true;
        else
            isavail=false;
        Refs.clear();
        while (1)
        {
            s=st.readLine();
            if (s.isEmpty())
                break;
            Refs.append(s.data());
        }
        f.close();
    }
}

int Article::score()
{
    return 100;
}


////////////////////////////////////////////////////////////////////
// NewsGroup class. Represents a newsgroup
// Real docs soon.
////////////////////////////////////////////////////////////////////


NewsGroup::NewsGroup(const char *name)
    :QString(name)
{
    isVisible=false;
    isTagged=false;
    lastArticle=0;
}


NewsGroup::~NewsGroup()
{
}

void NewsGroup::getList()
{
    int c=0;
    QString ID;
    artList.clear();
    QString ac;
    ac=krnpath+data();
    QFile f(ac);
    if (f.open(IO_ReadOnly))
    {
        QTextStream st(&f);
        while (!st.eof())
        {
            c++;
            if (!(c%100))
                qApp->processEvents();
            ID=st.readLine();
            if (ID.isEmpty())
                break;
            Article *spart=artSpool.find(ID.data());
            if (spart==NULL)
            {
                Article *art=new Article();
                art->ID=ID;
                art->load();
                artSpool.insert(ID.data(),art);
                artList.append(art);
            }
            else
            {
                artList.append(spart);
            }
        }
    }
}

void NewsGroup::updateList()
{
}

void NewsGroup::save()
{
    QString p=groupinfopath+data();
    QFile f(p.data());
    if(f.open (IO_WriteOnly))
    {
        QTextStream st(&f);
        st<<lastArticle;
        f.close();
    }
}

void NewsGroup::load()
{
    QString p=groupinfopath+data();
    QFile f(p.data());
    if(f.open (IO_ReadOnly))
    {
        QTextStream st(&f);
        lastArticle=st.readLine().toInt();
        f.close();
    }
}

void NewsGroup::getSubjects(NNTP *server)
{
    load();
    if (strcmp(server->group(),data()))
    {
        server->setGroup(data());
    }
    if (server->last>lastArticle)
    {
        debug ("xover from %d to %d",lastArticle+1,server->last);
        server->artList(lastArticle,server->last);
        lastArticle=server->last;
        save();
    }
}

void NewsGroup::getMessages(NNTP *server)
{
    debug ("getting articles in %s",data());
    load();
    getSubjects(server);
    getList();
    server->resetCounters (true,true);
    server->reportCounters (false,true);
    for (Article *art=artList.first();art!=0;art=artList.next())
    {
        if (!server->isCached(art->ID.data()))
            server->article(art->ID.data());
        qApp->processEvents();
    }
    server->resetCounters (true,true);
    server->reportCounters (true,false);
}

////////////////////////////////////////////////////////////////////
// ArtList class. Represents a list of articles
// Real docs soon.
////////////////////////////////////////////////////////////////////

void ArticleList::append(Article *item)
{
    item->incref();
    ArticleListBase::append((const Article *)item);
}

bool ArticleList::remove(uint index)
{
    Article *art=at(index);
    bool b=ArticleListBase::remove(index);
    if (b)
        art->decref();
    return b;
}

bool ArticleList::remove()
{
    return remove(at());
}

void ArticleList::clear()
{
    while (!isEmpty())
        remove(0);
}

QString noRe(QString subject)
{
    if (subject.left(3)=="Re:")
        subject=subject.right(subject.length()-3);
    if (subject.left(1)==" ")
        subject=subject.right(subject.length()-1);
    return subject;
}


bool isParent(Article *parent,Article *child)
{
    if (child->Refs.contains(parent->ID.data()))
        return true;
    else
        return false;
}

void collectChildren(ArticleList *parentThread,QList<ArticleList> *children)
{
    QListIterator <ArticleList> it(*children);
    for (;it.current();++it)
    {
        if (it.current()->isEmpty())
            //thread has already been adopted
            continue;
        ArticleList *childThread=it.current();
        if (isParent(parentThread->first(),childThread->first()))
        {
            //It's parent's daughter, make it collect its own kids,
            collectChildren(childThread,children);
            //and then adopt it
            QListIterator <Article> it2(*childThread);
            it2.toFirst();
            for (;it2.current();++it2)
            {
                it2.current()->threadDepth++;
                parentThread->append(it2.current());
            }
            childThread->clear();
        }
    }
}


void ArticleList::thread(bool sortBySubject=false)
{
    if (count()<2)
    {
        return; //not much to thread
    }
    QList <ArticleList> threads;
    QListIterator <ArticleList> thread(threads);
    threads.setAutoDelete(false);
    Article *iter;
    //Make a thread for each article
    //And set their depth to 0
    for (iter=this->first();iter!=0;iter=this->next())
    {
        ArticleList *l=new ArticleList;
        l->append(iter);
        threads.append(l);
        iter->threadDepth=0;
    }

    thread.toFirst();
    //Now consolidate threads
    ArticleList *parentThread;
    thread.toFirst();
    for (;thread.current();++thread)
    {
        parentThread=thread.current();
        if (thread.current()->isEmpty())
            //thread has already been adopted
            continue;
        //look for current's children
        collectChildren(parentThread,&threads);
    }
    clear();

    //If requested, sort the threads by subject
    QList <ArticleList> sortedThreads;
    if (sortBySubject)
    {
        thread.toFirst();
        for (;thread.current();++thread)
        {
            if (thread.current()->isEmpty())
                continue;
            uint i;
            for (i=0;i<sortedThreads.count();i++)
            {
                if (noRe(sortedThreads.at(i)->first()->Subject)
                    >=noRe(thread.current()->first()->Subject).data())
                    //hijole with the condition!
                    break;
            }
            if (i<sortedThreads.count())
                sortedThreads.insert (i,thread.current());
            else
                sortedThreads.append (thread.current());
        }
        threads=sortedThreads;
    }

    //Now thread the subthreads
    //And rebuild the list from them
    thread.toFirst();
    for (;thread.current();++thread)
    {
        //The first one is a root for sure
        if (!thread.current()->isEmpty())
        {
            QListIterator <Article> it2(*thread.current());
            for (;it2.current();++it2)
            {
                append(it2.current());
            }
        }
    }
}

ArticleList::~ArticleList()
{
    clear();
}

////////////////////////////////////////////////////////////////////
// GroupList class. Represents a list of newsgroup
// Real docs soon.
////////////////////////////////////////////////////////////////////


GroupList::GroupList()
{
}

GroupList::~GroupList()
{
}
int GroupList::compareItems(GCI item1,GCI item2)
{
    return strcmp(((NewsGroup *)item1)->data(),((NewsGroup *)item2)->data());
}


ArticleDict::ArticleDict ()
{
    setAutoDelete(true);
};
