/*
    Tanks - Tank arcade game
    Copyright (c) 2005 ProZroks

    <<< LICENSE >>>

    ProZroks
    support@prozroks.com

    Last modifed: Restorer (16 Jun, 2005)
*/

/*
    Whatsnew.

    Restorer (24 May, 2005)
        Только начало.

    Restorer (2 Jun, 2005)
        [add] Load
        [fix] Save

    Restorer (8 Jun, 2005)
    	[add] FindItem, CreateNode
        [add] GetInt, SetInt, GetString, SetString, GetBool, SetBool, Remove

    Restorer (16 Jun, 2005)
	    [fix] Теперь адекватно реагирует на TAB (код 9)
*/

#include "config.h"
#include "defines.h"
#include "exceptions.h"
#include <string.h>

C_Config::C_Config()
{
	root = NULL;
}

C_Config::~C_Config()
{
	if (root) DeleteTree(root);
}

void C_Config::DeleteTree(s_config_item *f)
{
	s_config_item *g;

	while (f)
	{
		if (f->child) DeleteTree(f->child);
		if (f->value) delete[] f->value;
		if (f->name) delete[] f->name;
		g = f;
		f = f->next;
		delete g;
	}
}

void C_Config::Save(const char *filename)
{
	C_File fl;

	fl.Write(filename);
	SaveItem(fl, root, 0);
	fl.Close();
}

void C_Config::SaveItem(C_File &fl, s_config_item *f, int depth)
{
	int i;

	while (f)
	{
		if (f->child)
		{
			for (i = 0; i < depth; i++) fl.PrintF("  "); fl.PrintF("<%s>\n", f->name);
			if (f->value)
			{
				for (i = 0; i < depth+1; i++) fl.PrintF("  ");
				for (i = 0; (unsigned)i < strlen(f->value); i++) if (f->value[i] == ' ') break;
				if ((unsigned)i == strlen(f->value)) fl.PrintF("%s\n", f->value);
				else fl.PrintF("\"%s\"\n", f->value);
			}
			SaveItem(fl, f->child, depth+1);
			for (i = 0; i < depth; i++) fl.PrintF("  "); fl.PrintF("</%s>\n", f->name);
		}
		else
		{
			for (i = 0; i < depth; i++) fl.PrintF("  ");
			fl.PrintF("<%s>", f->name);
			if (f->value)
			{
				for (i = 0; (unsigned)i < strlen(f->value); i++) if (f->value[i]==' ' || f->value[i]=='<' || f->value[i]=='>') break;
				if ((unsigned)i == strlen(f->value)) fl.PrintF("%s", f->value);
				else fl.PrintF("\"%s\"", f->value);
			}
			fl.PrintF("</%s>\n", f->name);
		}

		f = f->next;
	}
}

void C_Config::Load(const char *filename)
{
	C_File fl;

	if (root)
	{
		DeleteTree(root);
		root = NULL;
	}

	fl.Read(filename);
	root = LoadItem(fl);
}

C_Config::s_config_item* C_Config::LoadItem(C_File &fl)
{	
	int c = 0;
	unsigned o = 0;
	char buf[0x100];
	s_config_item *f = NULL;

	int state = 0;

	for (;;)
	{
		switch (state)
		{
			case 0:
				if (fl.Eof()) return NULL;
				c = fl.GetC();
				state = 1;
				break;

			case 1:		// skip leading spaces and EOLs
				if (fl.Eof()) return NULL;
				if (c!=' ' && c!=9 && c!=0x0A && c!=0x0D) {state = 2; break;}
				c = fl.GetC();
				break;

			case 2:		// test for '<'
				if (fl.Eof() || c!='<') throw C_E(E_XMLParserError);
				c = fl.GetC();
				state = 3;
				break;

			case 3:		// reading first symbol, if it is '/' return
				if (fl.Eof() || (c<=' ' && c!=9)) throw C_E(E_XMLParserError);
				if (c == '/') return NULL;
				buf[0] = c;
				o = 1;
				c = fl.GetC();
				state = 4;
				break;

			case 4:		// reading node name
				if (fl.Eof() || c<=' ' || c==9) throw C_E(E_XMLParserError);
				if (c == '>')
				{
					buf[o] = 0;
					f = new s_config_item;
					f->child = NULL;
					f->value = NULL;
					f->name = AllocNstrcpy(buf);
					c = fl.GetC();
					state = 5;
					break;
				}
				buf[o++] = c;
				if (o >= sizeof(buf)-1) throw C_E(E_XMLParserError, "Too long node name");
				c = fl.GetC();
				break;

			case 5:		// skip spaces
				if (fl.Eof()) throw C_E(E_XMLParserError);
				if (c!=' ' && c!=9 && c!=0x0A && c!=0x0D) {state = 6; break;}
				c = fl.GetC();
				break;

			case 6:		// get parameter
				if (c == '"') {c = fl.GetC(); o = 0; state = 7; break;}
				if (c == '<')
				{
					fl.UnGetC('<');
					f->child = LoadItem(fl);
					if (fl.Eof()) throw C_E(E_XMLParserError, "EOF");
					c = fl.GetC();
					o = 0;
					state = 11;
					break;
				}
				o = 0;
				state = 8;
				break;

			case 7:		// get <tag>"parameter"</tag>
				buf[o++] = c;
				if (o >= sizeof(buf)-1) throw C_E(E_XMLParserError, "Too long parameter");
				c = fl.GetC();
				if (fl.Eof() || (c<' ' && c!=9)) throw C_E(E_XMLParserError);
				if (c == '"')
				{
					buf[o] = 0;
					f->value = AllocNstrcpy(buf);
					c = fl.GetC();
					state = 9;
					break;
				}
				break;

			case 8:		// get <tag>parameter</tag>
				buf[o++] = c;
				if (o >= sizeof(buf)-1) throw C_E(E_XMLParserError, "Too long parameter");
				c = fl.GetC();
				if (fl.Eof()) throw C_E(E_XMLParserError);
				if (c==' ' || c==9 || c==0x0A || c==0x0D || c=='<')
				{
					buf[o] = 0;
					f->value = AllocNstrcpy(buf);
					state = 9;
					break;
				}
				break;

			case 9:		// skip spaces after parameter
				if (c == '<') {c = fl.GetC(); state = 10; break;}
				if (c!=' ' && c!=9 && c!=0x0A && c!=0x0D) throw C_E(E_XMLParserError);
				c = fl.GetC();
				break;

			case 10:	// test if it is closing tag
				if (c != '/') throw C_E(E_XMLParserError);
				c = fl.GetC();
				o = 0;
				state = 11;
				break;

			case 11:
				buf[o++] = c;
				if (o >= sizeof(buf)-1) throw C_E(E_XMLParserError, "Too long closing node name");
				c = fl.GetC();
				if (fl.Eof() || (c<=' ' && c!=9)) throw C_E(E_XMLParserError);
				if (c == '>')
				{
					buf[o] = 0;
					if (strcmp(f->name, buf)) throw C_E(E_XMLParserError, "Err 1");
					f->next = LoadItem(fl);
					return f;
				}
				break;
		}
	}
}

C_Config::s_config_item* C_Config::FindItem(const char *path, s_config_item*** prev)
{
	bool rp;
	s_config_item *f = root, **p = NULL;
	char itemName[0x100], *o;

	do
	{
		rp = false;

		o = itemName;
		while (*path && *path!='/' && (o-itemName)<(int)(sizeof(itemName)-1)) *(o++) = *(path++);
		if ((o-itemName) >= (int)(sizeof(itemName)-1)) throw C_E(E_ConfigError);
		*o = 0;

		while (f)
		{
			if (!stricmp(f->name, itemName)) break;
			p = &(f->next);
			f = f->next;
		}

		if (!f) return NULL;

		if (*path)
		{
			rp = true;
			path++;
			p = &(f->child);
			f = f->child;
		}
	} while (rp);

	if (prev) *prev = p;
	return f;
}

bool C_Config::GetString(const char *path, char **value)
{
	s_config_item *f;

	if (!(f = FindItem(path, NULL))) return false;
	if (!(f->value)) return false;

	*value = f->value;
	return true;
}

bool C_Config::GetBool(const char *path, bool *value)
{
	char *str;

	if (GetString(path, &str))
	{
		*value = (!stricmp(str, "true") || !stricmp(str, "yes"));
		return true;
	} else return false;
}

bool C_Config::GetInt(const char *path, int *value)
{
	char *str;

	if (GetString(path, &str))
	{
		*value = atoi(str);
		return true;
	} else return false;
}

C_Config::s_config_item* C_Config::CreateNode(const char *path)
{
	bool rp, fr;
	s_config_item *f = root, *p, **pp;
	char itemName[0x100], *o;

	fr = true;

	do
	{
		rp = false;

		o = itemName;
		while (*path && *path!='/' && (o-itemName)<(int)(sizeof(itemName)-1)) *(o++) = *(path++);
		if ((o-itemName) >= (int)(sizeof(itemName)-1)) throw C_E(E_ConfigError);
		*o = 0;
		p = f;

		while (f)
		{
			if (!stricmp(f->name, itemName)) break;
			p = f;
			f = f->next;
		}

		if (!f)
		{
			if (fr) return NULL;
			break;
		}

		if (*path)
		{
			fr = false;
			rp = true;
			path++;
			f = f->child;
		}
	} while (rp);

	if (*path)
	{
		pp = &(p->next);

		do
		{
			f = new s_config_item;
			f->name = AllocNstrcpy(itemName);
			f->next = NULL;
			f->value = NULL;
			*pp = f;
			pp = &(f->child);

			path++;
			o = itemName;
			while (*path && *path!='/' && (o-itemName)<(int)(sizeof(itemName)-1)) *(o++) = *(path++);
			if ((o-itemName) >= (int)(sizeof(itemName)-1)) throw C_E(E_ConfigError);
			*o = 0;
		}
		while (o != itemName);

		f->child = NULL;
	}
	else
	{
		if (!f)
		{
			f = new s_config_item;
			f->name = AllocNstrcpy(itemName);
			f->next = NULL;
			f->child = NULL;
			f->value = NULL;
			p->next = f;
		}
		else
		{
			if (f->value)
			{
				delete[] f->value;
				f->value = NULL;
			}
		}
	}

	return f;
}

void C_Config::SetString(const char *path, const char *value)
{
	s_config_item *f;

	if ((f = CreateNode(path)) != NULL)
	{
		f->value = AllocNstrcpy(value);
	} else throw C_E(E_ConfigError, "not \"root/...\"");
}

void C_Config::SetInt(const char *path, int value)
{
	char buf[0x100];
	sprintf(buf, "%d", value);
	SetString(path, buf);
}

void C_Config::SetBool(const char *path, bool value)
{
	if (value) SetString(path, "yes");
	else SetString(path, "no");
}

void C_Config::Remove(const char *path)
{
	s_config_item *f, **p, *pr;

	if (!(f = FindItem(path, &p))) return;
	if (!p) throw C_E(E_ConfigError, "can't remove root");

	pr = f->next;

	DeleteTree(f->child);
	if (f->value) delete[] f->value;
	if (f->name) delete[] f->name;
	delete f;

	*p = pr;
}

