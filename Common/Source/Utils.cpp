/*
XCSoar Glide Computer
Copyright (C) 2000 - 2004  M Roberts

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "stdafx.h"
#include "Utils.h"

#include "resource.h"
#include "externs.h"

#include "uniqueid.h"
#include "XCSoar.h"

#include <math.h>
#include <windows.h>
#include <tchar.h>

TCHAR szRegistryKey[] =                TEXT("Software\\MPSR\\XCSoar");
TCHAR szRegistrySpeedUnitsValue[] =    TEXT("Speed");
TCHAR szRegistryDistanceUnitsValue[] = TEXT("Distance");
TCHAR szRegistryAltitudeUnitsValue[] = TEXT("Altitude");
TCHAR szRegistryLiftUnitsValue[] =     TEXT("Lift");
TCHAR *szRegistryDisplayType[] =     { TEXT("Info0"),
																			 TEXT("Info1"),
																			 TEXT("Info2"),
																			 TEXT("Info3"),
																			 TEXT("Info4"),
																			 TEXT("Info5"),
																			 TEXT("Info6"),
																			 TEXT("Info7"),
																			 TEXT("Info8"),
																			 TEXT("Info9")
																		};						
TCHAR szRegistryDisplayUpValue[] =     TEXT("DisplayUp");																		
TCHAR szRegistryDisplayText[] =        TEXT("DisplayText");	
TCHAR szRegistrySafteyAltitude[] =     TEXT("SafteyAltitude");	
TCHAR szRegistryFAISector[] =					 TEXT("FAISector");
TCHAR szRegistrySectorRadius[]=        TEXT("Radius");
TCHAR szRegistryPolarID[] =					 TEXT("Polar");
TCHAR szRegistryWayPointFile[]=  TEXT("WPFile");
TCHAR szRegistryAirspaceFile[]=  TEXT("AirspaceFile");
TCHAR szRegistryTerrainFile[]=	 TEXT("TerrainFile");
TCHAR szRegistryAltMode[]=			 TEXT("AltitudeMode");
TCHAR szRegistryClipAlt[]=			 TEXT("ClipAlt");
TCHAR szRegistryAltMargin[]=		 TEXT("AltMargin");
TCHAR szRegistryRegKey[]=				 TEXT("RegKey");
TCHAR szRegistryPortIndex[]=		 TEXT("PortIndex");
TCHAR szRegistrySpeedIndex[]=		 TEXT("SpeedIndex");
TCHAR *szRegistryColour[] =     { TEXT("Colour0"),
																			 TEXT("Colour1"),
																			 TEXT("Colour2"),
																			 TEXT("Colour3"),
																			 TEXT("Colour4"),
																			 TEXT("Colour5"),
																			 TEXT("Colour6"),
																			 TEXT("Colour7"),
																			 TEXT("Colour8"),
																			 TEXT("Colour9"),
																			 TEXT("Colour10"),
																			 TEXT("Colour11"),
																			 TEXT("Colour12")
																		};		
TCHAR szRegistrySnailTrail[]=		 TEXT("SnailTrail");
TCHAR szRegistryStartLine[]=		 TEXT("StartLine");
TCHAR szRegistryStartRadius[]=			TEXT("StartRadius");
TCHAR szRegistryAirspaceWarning[]=			TEXT("AirspaceWarn");
TCHAR szRegistryWarningTime[]=					TEXT("WarnTime");
				

static double SINETABLE[910];

void StoreType(int Index,int InfoType)
{
	SetToRegistry(szRegistryDisplayType[Index],(DWORD)InfoType);
}
void ReadRegistrySettings(void)
{
	DWORD Speed = 0;
	DWORD Distance = 0;
	DWORD Lift = 0;
	DWORD Altitude = 0;
	DWORD DisplayUp = 0;
	DWORD Temp = 0;
	int i;

	GetFromRegistry(szRegistrySpeedUnitsValue,&Speed);
	switch(Speed)
	{
		case 0 : SPEEDMODIFY = TOMPH;	break;
		case 1 : SPEEDMODIFY = TOKNOTS; break;
		case 2 : SPEEDMODIFY = TOKPH; break;
	}
	
	GetFromRegistry(szRegistryDistanceUnitsValue,&Distance);
	switch(Distance)
	{
		case 0 : DISTANCEMODIFY = TOMILES; break;
		case 1 : DISTANCEMODIFY = TONAUTICALMILES; break;
		case 2 : DISTANCEMODIFY = TOKILOMETER; break;
	}
	
	GetFromRegistry(szRegistryAltitudeUnitsValue,&Altitude);
	switch(Altitude)
	{
		case 0 : ALTITUDEMODIFY = TOFEET; break;
		case 1 : ALTITUDEMODIFY = TOMETER; break;
	}
	
	GetFromRegistry(szRegistryLiftUnitsValue,&Lift);
	switch(Lift)
	{
		case 0 : LIFTMODIFY = TOKNOTS; break;
		case 1 : LIFTMODIFY = TOMETER; break;
	}
	
	for (i=0;i<NUMINFOWINDOWS;i++)
	{
		if(GetFromRegistry(szRegistryDisplayType[i],&Altitude) == ERROR_SUCCESS )
			InfoType[i] = Altitude;
	}
	
	GetFromRegistry(szRegistryDisplayUpValue,&Temp);
	switch(Temp)
	{
		case TRACKUP : DisplayOrientation = TRACKUP; break;
		case NORTHUP : DisplayOrientation = NORTHUP;break;
		case NORTHCIRCLE : DisplayOrientation = NORTHCIRCLE;break;
	}
	
	GetFromRegistry(szRegistryDisplayText,&Temp);
	switch(Temp)
	{
		case 0 : DisplayTextType = DISPLAYNAME; break;
		case 1 : DisplayTextType = DISPLAYNUMBER;break;
		case 2 : DisplayTextType = DISPLAYFIRSTFIVE; break;
		case 3 : DisplayTextType = DISPLAYNONE;break;
		case 4 : DisplayTextType = DISPLAYFIRSTTHREE; break;
	}
	
	if(GetFromRegistry(szRegistryAltMode,&Temp)==ERROR_SUCCESS)
		AltitudeMode = Temp;

	if(GetFromRegistry(szRegistryClipAlt,&Temp)==ERROR_SUCCESS)
		ClipAltitude = Temp;

	if(GetFromRegistry(szRegistryAltMargin,&Temp)==ERROR_SUCCESS)
		AltWarningMargin = Temp;

	
	GetFromRegistry(szRegistrySafteyAltitude,&Temp);
	if(Temp != 0)
		SAFTEYALTITUDE = (double)Temp;

	Temp = 0;
	GetFromRegistry(szRegistryFAISector,&Temp);
	FAISector = Temp;

	GetFromRegistry(szRegistrySectorRadius,&SectorRadius);
	

	GetFromRegistry(szRegistryPolarID,&Temp); POLARID = (int)Temp;

	
	GetRegistryString(szRegistryRegKey, strRegKey, 65);

	for(i=0;i<12;i++)
	{
		if(GetFromRegistry(szRegistryColour[i],&Temp)==ERROR_SUCCESS)
			iAirspaceBrush[i] =			(int)Temp;
		else
			iAirspaceBrush[i] =			0;
	}
	
	GetFromRegistry(szRegistrySnailTrail,&Temp);
	TrailActive = Temp;

	Temp = 1;
	GetFromRegistry(szRegistryStartLine,&Temp);
	StartLine = Temp;

	Temp = 1000;
	GetFromRegistry(szRegistryStartRadius,&Temp);
	StartRadius = Temp;

	Temp = 0;
	GetFromRegistry(szRegistryAirspaceWarning,&Temp);
	AIRSPACEWARNINGS = Temp;

	Temp = 30;
	GetFromRegistry(szRegistryWarningTime,&Temp);
	WarningTime = Temp;
}




BOOL GetFromRegistry(const TCHAR *szRegValue, DWORD *pPos)    
{
	HKEY    hKey;
	DWORD    dwSize, dwType;
	long    hRes;

  *pPos= 0;        
  hRes = RegOpenKeyEx(HKEY_CURRENT_USER, szRegistryKey, 0, KEY_ALL_ACCESS, &hKey);
  if (hRes != ERROR_SUCCESS) 
	{
		RegCloseKey(hKey);
    return hRes;
  }

  dwSize = sizeof(DWORD);
  hRes = RegQueryValueEx(hKey, szRegValue, 0, &dwType, (LPBYTE)pPos, &dwSize);
  RegCloseKey(hKey);
  return hRes;
}


// Implement your code to save value to the regsitry

HRESULT SetToRegistry(const TCHAR *szRegValue, DWORD Pos)    
{
	HKEY    hKey;
	DWORD    Disp;
	HRESULT hRes;

  hRes = RegCreateKeyEx(HKEY_CURRENT_USER, szRegistryKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &Disp);
  if (hRes != ERROR_SUCCESS) 
	{
			return FALSE;
  }

  hRes = RegSetValueEx(hKey, szRegValue,0,REG_DWORD, (LPBYTE)&Pos, sizeof(DWORD));
  RegCloseKey(hKey);                    

  return hRes;
}

BOOL GetRegistryString(const TCHAR *szRegValue, TCHAR *pPos, DWORD dwSize)    
{
	HKEY    hKey;
	DWORD   dwType = REG_SZ;
	long    hRes;

  pPos[0]= '\0';        
  hRes = RegOpenKeyEx(HKEY_CURRENT_USER, szRegistryKey, 0, KEY_ALL_ACCESS, &hKey);
  if (hRes != ERROR_SUCCESS) 
	{
		RegCloseKey(hKey);
    return FALSE;
  }

  hRes = RegQueryValueEx(hKey, szRegValue, 0, &dwType, (LPBYTE)pPos, &dwSize);
  RegCloseKey(hKey);
  return hRes;
}

HRESULT SetRegistryString(const TCHAR *szRegValue, TCHAR *Pos)    
{
	HKEY    hKey;
	DWORD    Disp;
	HRESULT hRes;

  hRes = RegCreateKeyEx(HKEY_CURRENT_USER, szRegistryKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &Disp);
  if (hRes != ERROR_SUCCESS) 
	{
			return FALSE;
  }

  hRes = RegSetValueEx(hKey, szRegValue,0,REG_SZ, (LPBYTE)Pos, _tcslen(Pos)*sizeof(TCHAR));
  RegCloseKey(hKey);                    

  return hRes;
}

void ReadPortSettings(DWORD *PortIndex, DWORD *SpeedIndex)
{
	DWORD Temp;

	if(GetFromRegistry(szRegistryPortIndex,&Temp)==ERROR_SUCCESS)
		(*PortIndex) = Temp;

	if(GetFromRegistry(szRegistrySpeedIndex,&Temp)==ERROR_SUCCESS)
		(*SpeedIndex) = Temp;
}

void WritePortSettings(DWORD PortIndex, DWORD SpeedIndex)
{
	SetToRegistry(szRegistryPortIndex, PortIndex);
	SetToRegistry(szRegistrySpeedIndex, SpeedIndex);
}

		
void rotate(double *xin, double *yin, double angle)
{
	static double x,y;
	static double xout, yout;
	static double lastangle = 0;
	static double cost=1,sint=0;
	
	if(angle != lastangle)
	{
		lastangle = angle;
		cost = (double)fastcosine(angle);
		sint = (double)fastsine(angle);
	}
	
	x = *xin; y = *yin;
	xout = x*cost - y*sint;
	yout = y*cost + x*sint;
	*xin = xout;
	*yin = yout;
}
			
			
double Distance(double lat1, double lon1, double lat2, double lon2)
{
	double angle;

	lat1 = (lat1/180) * pi;
	lat2 = (lat2/180) * pi;
	lon1 = (lon1/180) * pi;
	lon2 = (lon2/180) * pi;

	angle = (double)acos( sin(lat1)*sin(lat2) +  cos(lat1)*cos(lat2) * cos(lon1-lon2)  );
	angle = angle * (180/pi);

	return (double)(angle * 111194.9267);
}

double Bearing(double lat1, double lon1, double lat2, double lon2)
{
	double angle;
	double d;

	lat1 = (lat1/180) * pi;
	lat2 = (lat2/180) * pi;
	lon1 = (lon1/180) * pi;
	lon2 = (lon2/180) * pi;
	
	d = (sin(lat1)*sin(lat2) +  cos(lat1)*cos(lat2) * cos(lon1-lon2) );
	if(d>1) d = 0.99999999999999;
	if(d<-1) d = -0.99999999999999;
	d = acos(d);

	if(sin(lon1-lon2)<0 )      
	{
		angle = (((sin(lat2)-sin(lat1)) * cos(d) ) / (sin(d)*cos(lat1)));
		
		if(angle >1) angle = 1;
		if(angle<-1) angle = -1;
		angle = acos(angle);

		if(lat1>lat2)
			angle = angle * (180/pi);
		else
			angle = angle * (180/pi);
	}
	else       
	{
		angle=(( (sin(lat2)-sin(lat1)) * cos(d) ) / (sin(d)*cos(lat1)));
		if(angle >1) angle = 1;
		if(angle<-1) angle = -1;
		angle = acos(angle);

		angle = 360 - (angle * (180/pi));
	}

	return (double)angle;
}
double Reciprocal(double InBound)
{
	if(InBound >= 180)
		return InBound - 180;
	else
		return InBound + 180;
}

double BiSector(double InBound, double OutBound)
{
	double result;


	InBound = Reciprocal(InBound);

	if(InBound == OutBound)
	{
		result = Reciprocal(InBound);
	}
	
	else if (InBound > OutBound)
	{
		if( (InBound - OutBound) < 180)
		{
			result = Reciprocal((InBound+OutBound)/2);
		}
		else
		{
			result = (InBound+OutBound)/2;
		}
	}
	else
	{
		if( (OutBound - InBound) < 180)
		{
			result = Reciprocal((InBound+OutBound)/2);
		}
		else
		{
			result = (InBound+OutBound)/2;
		}
	}
	return result;
}

void CalculateNewPolarCoef(void)
{
	static double Polars[6][3] = {{-0.0538770500225782443497, 0.1323114348, -0.1273364037098239098543}, 
																{-0.0532456270195884696748, 0.1509454717, -0.1474304674787072275183}, 
																{-0.0598306909918491529791, 0.1896480967, -0.1883344146619101871894}, 
																{-0.0303118230885946660507, 0.0771466019, -0.0799469636558217515699}, 
																{-0.0222929913566948641563, 0.0318771616, -0.0307925896846546928318}, 
																{-0.0430828898445299480353, 0.0746938776, -0.0487285153053357557183}
															};

	static double Weights[6][3] = {{70,190,1}, 
																 {70,250,100}, 
																 {70,240,285}, 
																 {70,287,165}, 
																 {70,400,120}, 
																 {70,527,303} };
	int i;

	for(i=0;i<3;i++)
	{
		POLAR[i] = Polars[POLARID][i];
		WEIGHTS[i] = Weights[POLARID][i];
	}

/*	double d;
	double v1,v2,v3;
	double w1,w2,w3;

	v1 = POLARV[0]; v2 = POLARV[1]; v3 = POLARV[2];
	w1 = -POLARV[0]/POLARLD[0]; w2 = -POLARV[1]/POLARLD[1]; w3 = -POLARV[2]/POLARLD[2];

	d = v1*v1*(v2-v3)+v2*v2*(v3-v1)+v3*v3*(v1-v2);
	if (d == 0.0) 
	{
		POLAR[0]=0;
	} 
	else
	{
		POLAR[0]=((v2-v3)*(w1-w3)+(v3-v1)*(w2-w3))/d;
	}
	d = v2-v3;
	if (d == 0.0) 
	{
		POLAR[1]=0;
	}
	else
	{
		POLAR[1] = (w2-w3-POLAR[0]*(v2*v2-v3*v3))/d;
	}	 
	
	POLAR[2] = (double)(w3 - POLAR[0] *v3*v3 - POLAR[1]*v3);
	POLAR[0] = POLAR[0] * (double)sqrt(WEIGHTS[0] + WEIGHTS[1]);
	POLAR[2] = POLAR[2] / (double)sqrt(WEIGHTS[0] + WEIGHTS[1]);*/
}

int Registered(void)
{
	int Raw[8];
	int i;
	int Year, Month;

	_stscanf(strRegKey,TEXT("%02X%02X%02X%02X%02X%02X%02X%02X"),&Raw[0],&Raw[1],&Raw[2],&Raw[3],&Raw[4],&Raw[5],&Raw[6],&Raw[7]);

	for(i=0;i<8;i++)
	{
		Raw[i] = Raw[i] ^ (strAssetNumber[i] + i);
	}

	if(Raw[0] != Raw[4]) return FALSE;
	if(Raw[1] != Raw[5]) return FALSE;
	if(Raw[2] != Raw[6]) return FALSE;
	if(Raw[3] != Raw[7]) return FALSE;

	Month = MAKEWORD(Raw[0],Raw[1]);
	Year = MAKEWORD(Raw[2],Raw[3]);
	
	if(GPS_INFO.Year > Year) return FALSE;
	if((GPS_INFO.Year  == Year)&&(GPS_INFO.Month > Month)) return FALSE;

	return TRUE; 
}

void CheckRegistration(void)
{
	static BOOL Warning = FALSE;
	static int RegistrationTimeOut = 150;
	static BOOL LOCK = FALSE;
	static int LastYear = 0, LastMonth = 0, LastDay = 0;


	if(LOCK)
	{
		return;
	}

	LOCK = TRUE;

	if((GPS_INFO.Year == 0) && (GPS_INFO.Month  == 0) && (GPS_INFO.Day  == 0))
	{
		LOCK = FALSE;
		LastYear = GPS_INFO.Year; LastMonth = GPS_INFO.Month; LastDay = GPS_INFO.Day; 
		return;
	}

	if(GPS_INFO.Year <= 2004)
	{
		LOCK = FALSE;
		LastYear = GPS_INFO.Year; LastMonth = GPS_INFO.Month; LastDay = GPS_INFO.Day; 
		
		return;
	}

	if((GPS_INFO.Year == 2003) && (GPS_INFO.Month  < 3))
	{
		LOCK = FALSE;
		LastYear = GPS_INFO.Year; LastMonth = GPS_INFO.Month; LastDay = GPS_INFO.Day; 
		return;
	}

	if(Registered())
	{
		LastYear = GPS_INFO.Year; LastMonth = GPS_INFO.Month; LastDay = GPS_INFO.Day; 
		LOCK = FALSE;
		return;
	}

	if((LastYear != GPS_INFO.Year)||(LastMonth != GPS_INFO.Month)||(LastDay = GPS_INFO.Day))
	{
		LastYear = GPS_INFO.Year; LastMonth = GPS_INFO.Month; LastDay = GPS_INFO.Day; 
		LOCK = FALSE;
		return;
	}	

	LastYear = GPS_INFO.Year; LastMonth = GPS_INFO.Month; LastDay = GPS_INFO.Day; 
	
	if(Warning == FALSE)
	{
		MessageBox(hWndMainWindow,TEXT("Your Registration Key Has Expired Or No Registration Key Was Found.  See File->Register To Set A New Key.  Trial Use Limited To 10 Mins"),TEXT("Trial Use"),MB_OK|MB_ICONINFORMATION|MB_APPLMODAL|MB_TOPMOST);
		Warning = TRUE;
	}

	if(RegistrationTimeOut >0)
	{
		RegistrationTimeOut --;
		LOCK = FALSE;
		return;
	}
	else
	{
			MessageBox(hWndMainWindow,TEXT("Trial Period Has Expired.  See File->Register To Set A New Key"),TEXT("Trail Period Expired"),MB_OK|MB_ICONSTOP|MB_APPLMODAL|MB_TOPMOST);
			SendMessage(hWndMainWindow, WM_ACTIVATE, MAKEWPARAM(WA_INACTIVE, 0), (LPARAM)hWndMainWindow);
			SendMessage (hWndMainWindow, WM_CLOSE, 0, 0);	
	}
	LOCK = FALSE;
}

void CalculateTaskSectors(void)
{
	int i;
	double SectorAngle, SectorSize, SectorBearing;
	
	for(i=0;i<MAXTASKPOINTS-1;i++)
	{
		if((Task[i].Index >=0) &&  (Task[i+1].Index >=0))
		{
			if(i == 0)
			{
				SectorAngle = 90;
				SectorSize = StartRadius;
				SectorBearing = Task[i].OutBound;
			}
			else
			{
				SectorAngle = 45;
				SectorSize = 5000;
				SectorBearing = Task[i].Bisector;
			}

			Task[i].SectorStartLat = FindLattitude(WayPointList[Task[i].Index].Lattitude,WayPointList[Task[i].Index].Longditude, SectorBearing + SectorAngle, SectorSize);
			Task[i].SectorStartLon = FindLongditude(Task[i].SectorStartLat,WayPointList[Task[i].Index].Longditude, SectorBearing + SectorAngle, SectorSize);
			Task[i].SectorEndLat   = FindLattitude(WayPointList[Task[i].Index].Lattitude,WayPointList[Task[i].Index].Longditude,	SectorBearing - SectorAngle, SectorSize);
			Task[i].SectorEndLon   = FindLongditude(Task[i].SectorEndLat,WayPointList[Task[i].Index].Longditude,	SectorBearing - SectorAngle, SectorSize);
		}
	}
}

void CalculateAATTaskSectors(void)
{
	int i;
	
	if(AATEnabled == FALSE)
		return;

	for(i=1;i<MAXTASKPOINTS-1;i++)
	{
		if((Task[i].Index >=0))
		{
			if(Task[i].AATType == SECTOR)
			{
				Task[i].AATStartLat = FindLattitude (WayPointList[Task[i].Index].Lattitude,WayPointList[Task[i].Index].Longditude, Task[i].AATStartRadial , Task[i].AATSectorRadius );
				Task[i].AATStartLon = FindLongditude(WayPointList[Task[i].Index].Lattitude,WayPointList[Task[i].Index].Longditude, Task[i].AATStartRadial , Task[i].AATSectorRadius );
				Task[i].AATFinishLat= FindLattitude (WayPointList[Task[i].Index].Lattitude,WayPointList[Task[i].Index].Longditude,	Task[i].AATFinishRadial , Task[i].AATSectorRadius );
				Task[i].AATFinishLon= FindLongditude(WayPointList[Task[i].Index].Lattitude,WayPointList[Task[i].Index].Longditude,	Task[i].AATFinishRadial , Task[i].AATSectorRadius );
			}
		}
	}
}


double FindLattitude(double Lat, double Lon, double Bearing, double Distance)
{
	double result;

	Lat = (Lat/180) * pi;
	Lon = (Lon/180) * pi;
	Bearing = (Bearing/180) * pi;

	Distance = Distance/6371000;
 
	result = (double)asin(sin(Lat)*cos(Distance)+cos(Lat)*sin(Distance)*cos(Bearing));
	return result * (180/pi);;
}

double FindLongditude(double Lat, double Lon, double Bearing, double Distance)
{
	double result;

	Lat = (Lat/180) * pi;
	Lon = (Lon/180) * pi;
	Bearing = (Bearing/180) * pi;

	Distance = Distance/6371000;

	if(cos(Lat)==0)
		result = Lon;
  else
	{
		result = Lon+(double)asin(sin(Bearing)*sin(Distance)/cos(Lat));
		result = (double)fmod((result+pi),(2*pi));
		result = result - pi;
	}
  return result * (180/pi);
}

static double xcoords[64] = {
	0,			0.09801714,		0.195090322,	0.290284677,	0.382683432,	0.471396737,	0.555570233,	0.634393284,
	0.707106781,	0.773010453,	0.831469612,	0.881921264,	0.923879533,	0.956940336,	0.98078528,		0.995184727,
	1,			0.995184727,	0.98078528,		0.956940336,	0.923879533,	0.881921264,	0.831469612,	0.773010453,
	0.707106781,	0.634393284,	0.555570233,	0.471396737,	0.382683432,	0.290284677,	0.195090322,	0.09801714,
	0,			-0.09801714,	-0.195090322,	-0.290284677,	-0.382683432,	-0.471396737,	-0.555570233,	-0.634393284,
	-0.707106781,	-0.773010453,	-0.831469612,	-0.881921264,	-0.923879533,	-0.956940336,	-0.98078528,	-0.995184727,
	-1,			-0.995184727,	-0.98078528,	-0.956940336,	-0.923879533,	-0.881921264,	-0.831469612,	-0.773010453,
	-0.707106781,	-0.634393284,	-0.555570233,	-0.471396737,	-0.382683432,	-0.290284677,	-0.195090322,	-0.09801714
};

static double ycoords[64] = {
	1,			0.995184727,	0.98078528,		0.956940336,	0.923879533,	0.881921264,	0.831469612,	0.773010453,
	0.707106781,	0.634393284,	0.555570233,	0.471396737,	0.382683432,	0.290284677,	0.195090322,	0.09801714,
	0,			-0.09801714,	-0.195090322,	-0.290284677,	-0.382683432,	-0.471396737,	-0.555570233,	-0.634393284,
	-0.707106781,	-0.773010453,	-0.831469612,	-0.881921264,	-0.923879533,	-0.956940336,	-0.98078528,	-0.995184727,
	-1,			-0.995184727,	-0.98078528,	-0.956940336,	-0.923879533,	-0.881921264,	-0.831469612,	-0.773010453,
	-0.707106781,	-0.634393284,	-0.555570233,	-0.471396737,	-0.382683432,	-0.290284677,	-0.195090322,	-0.09801714,
	0,			0.09801714,		0.195090322,	0.290284677,	0.382683432,	0.471396737,	0.555570233,	0.634393284,
	0.707106781,	0.773010453,	0.831469612,	0.881921264,	0.923879533,	0.956940336,	0.98078528,		0.995184727
};


int Circle(HDC hdc, long x, long y, int radius, RECT rc)
{
	POINT pt[65];
	int i;
	
	for(i=0;i<64;i++)
	{
		pt[i].x = x + (long) (radius * xcoords[i]);
		pt[i].y = y + (long) (radius * ycoords[i]);
	}
	pt[64].x = x + (long) (radius * xcoords[0]);
	pt[64].y = y + (long) (radius * ycoords[0]);
	
	if(PolygonVisible(pt, 65, rc))
	{
		Polygon(hdc,pt,65);
		return true;
	}
	else
	{
		return false;
	}
}
	
void ConvertFlightLevels(void)
{
	unsigned i;

	for(i=0;i<NumberOfAirspaceCircles;i++)
	{
		if(AirspaceCircle[i].Base.FL  != 0)
		{
			AirspaceCircle[i].Base.Altitude = (AirspaceCircle[i].Base.FL * 100) + ((QNH-1013)*30);
			AirspaceCircle[i].Base.Altitude = AirspaceCircle[i].Base.Altitude / TOFEET;
		}
		if(AirspaceCircle[i].Top.FL  != 0)
		{
			AirspaceCircle[i].Top.Altitude = (AirspaceCircle[i].Top.FL * 100) + ((QNH-1013)*30);
			AirspaceCircle[i].Top.Altitude = AirspaceCircle[i].Top.Altitude / TOFEET;
		}
	}
		
		
	for(i=0;i<NumberOfAirspaceAreas;i++)
	{
		if(AirspaceArea[i].Base.FL  != 0)
		{
			AirspaceArea[i].Base.Altitude = (AirspaceArea[i].Base.FL * 100) + ((QNH-1013)*30);
			AirspaceArea[i].Base.Altitude = AirspaceArea[i].Base.Altitude / TOFEET;
		}
		if(AirspaceArea[i].Top.FL  != 0)
		{
			AirspaceArea[i].Top.Altitude = (AirspaceArea[i].Top.FL * 100) + ((QNH-1013)*30);
			AirspaceArea[i].Top.Altitude = AirspaceArea[i].Top.Altitude / TOFEET;
		}
	}
}

BOOL PolygonVisible(const POINT *lpPoints, int nCount, RECT rc)
{
	BOOL Sector[9] = {FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
	int i;
	int Count = 0;

	//return TRUE;

	for(i=0;i<nCount;i++)
	{
		if(lpPoints[i].y < 0)
		{
			if(lpPoints[i].x <0)
			{
				Sector[0] = TRUE;
			}
			else if((lpPoints[i].x >=0) && (lpPoints[i].x <240))
			{
				Sector[1] = TRUE;
			}
			else if(lpPoints[i].x >=240)
			{
				Sector[2] = TRUE;
			}
		}
		else if((lpPoints[i].y >=0) && (lpPoints[i].y <320))
		{
			if(lpPoints[i].x <0)
			{
				Sector[3] = TRUE;
			}
			else if((lpPoints[i].x >=0) && (lpPoints[i].x <240))
			{
				Sector[4] = TRUE;
				return TRUE;
			}
			else if(lpPoints[i].x >=240)
			{
				Sector[5] = TRUE;
			}
		}
		else if(lpPoints[i].y >=320)
		{
			if(lpPoints[i].x <0)
			{
				Sector[6] = TRUE;
			}
			else if((lpPoints[i].x >=0) && (lpPoints[i].x <240))
			{
				Sector[7] = TRUE;
			}
			else if(lpPoints[i].x >=240)
			{
				Sector[8] = TRUE;
			}
		}
	}	

	for(i=0;i<9;i++)
	{
		if(Sector[i])
		{
			Count ++;
		}
	}

	if(Count>= 2)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void SetRegistryColour(int i, DWORD c)
{
	SetToRegistry(szRegistryColour[i] ,c) ;
}



void ReadAssetNumber(void)
{
	if(strAssetNumber[0] != '\0')
	{
		return;
	}

	ReadUUID();

	if(strAssetNumber[0] != '\0')
	{
		return;
	}

	ReadCompaqID();
}

void ReadCompaqID(void)
{
	PROCESS_INFORMATION pi;
	HANDLE hInFile;// = INVALID_HANDLE_VALUE; 
	DWORD dwBytesRead;   

	if(strAssetNumber[0] != '\0')
	{
		return;
	}

	CreateProcess(TEXT("\\windows\\CreateAssetFile.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi); 
	
	hInFile = CreateFile(TEXT("\\windows\\cpqAssetData.dat"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0); 
	if (hInFile == INVALID_HANDLE_VALUE) 
	{ 
//	    MessageBox(hWnd, TEXT("Unable to open asset data file."), TEXT("Error!"), MB_OK); 
		  return; 
	} 
	SetFilePointer(hInFile, 976, NULL, FILE_BEGIN); 
	memset(strAssetNumber, 0, 64 * sizeof(TCHAR)); 
	ReadFile(hInFile, &strAssetNumber, 64, &dwBytesRead, NULL); 
	CloseHandle(hInFile);
}

void ReadUUID(void)
{
	BOOL fRes;
  DWORD dwBytesReturned =0;
	DEVICE_ID* pDevID;
	int wSize;
	int i;

	GUID Guid;

	unsigned long temp, Asset;

	memset(&Guid, 0, sizeof(GUID));

	pDevID = (DEVICE_ID*)malloc(sizeof(DEVICE_ID));
	memset(pDevID, 0, sizeof(DEVICE_ID));
	pDevID->dwSize = sizeof(DEVICE_ID);

	fRes = KernelIoControl( IOCTL_HAL_GET_DEVICEID, NULL, 0,
            pDevID, sizeof( DEVICE_ID ), &dwBytesReturned );

	wSize = pDevID->dwSize;
	free(pDevID); pDevID = NULL;
	
	if( (FALSE != fRes) || (ERROR_INSUFFICIENT_BUFFER != GetLastError()))
		return;

	pDevID = (DEVICE_ID*)malloc(sizeof(wSize));
	memset(pDevID, 0, sizeof(wSize));
	pDevID->dwSize = wSize;

  fRes = KernelIoControl( IOCTL_HAL_GET_DEVICEID, NULL, 0,
            pDevID, wSize, &dwBytesReturned );

	if((FALSE == fRes) || (ERROR_INSUFFICIENT_BUFFER == GetLastError()) )
		return;

	BYTE* pDat = (BYTE*)&Guid.Data1;
	BYTE* pSrc = (BYTE*)(pDevID) + pDevID->dwPresetIDOffset;
	memcpy(pDat, pSrc, pDevID->dwPresetIDBytes);
	pDat +=  pDevID->dwPresetIDBytes;
	pSrc =  (BYTE*)(pDevID) + pDevID->dwPlatformIDOffset;
	memcpy(pDat, pSrc, pDevID->dwPlatformIDBytes);
	

	temp = Guid.Data2; temp = temp << 16;
	temp += Guid.Data3 ;

	Asset = temp ^ Guid.Data1 ;

	temp = 0;
	for(i=0;i<4;i++)
	{
		temp = temp << 8;
		temp += Guid.Data4[i];
	}

	Asset = Asset ^ temp;

	temp = 0;
	for(i=0;i<4;i++)
	{
		temp = temp << 8;
		temp += Guid.Data4[i+4];
	}

	Asset = Asset ^ temp;

	wsprintf(strAssetNumber,TEXT("%08X%08X"),Asset,Guid.Data1 );
	return;
}

void WriteProfile(HWND hwnd, TCHAR *szFile)
{
	HANDLE hFile;
	DWORD dwBytesWritten;

	hFile = CreateFile(szFile,GENERIC_WRITE,0,(LPSECURITY_ATTRIBUTES)NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}

	WriteFile(hFile,InfoType,sizeof(InfoType),&dwBytesWritten,NULL);
	WriteFile(hFile,&POLARID,sizeof(POLARID),&dwBytesWritten,NULL);
	WriteFile(hFile,iAirspaceBrush,11*sizeof(iAirspaceBrush[0]),&dwBytesWritten,NULL);

	CloseHandle(hFile);
}

void ReadProfile(HWND hwnd, TCHAR *szFile)
{
	HANDLE hFile;
	DWORD dwBytesRead;
	int i;

	hFile = CreateFile(szFile,GENERIC_READ,0,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile == INVALID_HANDLE_VALUE )
	{
		return;
	}

	
	ReadFile(hFile,InfoType,sizeof(InfoType),&dwBytesRead,NULL);
	for(i=0;i<NUMINFOWINDOWS;i++)
	{
		StoreType(i,InfoType[i]);
	}

	ReadFile(hFile,&POLARID,sizeof(POLARID),&dwBytesRead,NULL);	
	CalculateNewPolarCoef();
	SetToRegistry(szRegistryPolarID,(DWORD)POLARID);
	
	ReadFile(hFile,iAirspaceBrush,11*sizeof(iAirspaceBrush[0]),&dwBytesRead,NULL);
	for(i=0;i<11;i++)
	{
		SetRegistryColour(i,iAirspaceBrush[i]);
	}

	CloseHandle(hFile);
}

double ScreenAngle(int x1, int y1, int x2, int y2)
{
	return 180* (atan2(y2-y1, x2-x1)/pi);
}

void FormatWarningString(int Type, TCHAR *Name , AIRSPACE_ALT Base, AIRSPACE_ALT Top, TCHAR *szMessageBuffer, TCHAR *szTitleBuffer )
{
	TCHAR BaseStr[512];
	TCHAR TopStr[512];

	switch (Type)
	{
		case RESTRICT:		_tcscpy(szTitleBuffer,TEXT("Restricted")); break;
		case PROHIBITED:	_tcscpy(szTitleBuffer,TEXT("Prohibited")); break;
		case DANGER:			_tcscpy(szTitleBuffer,TEXT("Danger Area")); break;
		case CLASSA:			_tcscpy(szTitleBuffer,TEXT("Class A")); break;
		case CLASSB:			_tcscpy(szTitleBuffer,TEXT("Class B")); break;
		case CLASSC:			_tcscpy(szTitleBuffer,TEXT("Class C")); break;
		case CLASSD:			_tcscpy(szTitleBuffer,TEXT("Class D")); break;
		case NOGLIDER:		_tcscpy(szTitleBuffer,TEXT("No Glider")); break;
		case CTR:					_tcscpy(szTitleBuffer,TEXT("CTR")); break;
		case WAVE:				_tcscpy(szTitleBuffer,TEXT("Wave")); break;
		default:					_tcscpy(szTitleBuffer,TEXT("Unknown"));
	}

	if(Base.FL == 0)
	{
		wsprintf(BaseStr,TEXT("%1.0f Alt"),ALTITUDEMODIFY * Base.Altitude );
	}
	else
	{
		wsprintf(BaseStr,TEXT("FL %1.0f"),Base.FL );
	}

	if(Top.FL == 0)
	{
		wsprintf(TopStr,TEXT("%1.0f Alt"),ALTITUDEMODIFY * Top.Altitude );
	}
	else
	{
		wsprintf(TopStr,TEXT("FL %1.0f"),Top.FL );
	}

	wsprintf(szMessageBuffer,TEXT("%s - %s\r\nBase - %s\r\nTop - %s"),szTitleBuffer, Name, BaseStr, TopStr);
}

BOOL ReadString(HANDLE hFile, int Max, TCHAR *String)
{
	int i;
	char c;
	DWORD dwNumBytesRead;

	for(i=0;i<Max;i++)
	{
		dwNumBytesRead = 0;
		ReadFile(hFile,&c,1,&dwNumBytesRead,NULL);
		if(dwNumBytesRead != 0)
		{
			if(c == '\n')
			{
				String[i] = '\0';
				return TRUE;
			}
			String[i] = (TCHAR)c;
		}
		else
		{
			if(i>0)
			{
				String[i] = 0;
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	String[i] = '\0';
	return TRUE;
}


void InitSineTable(void)
{
	int i;
	double angle;

	for(i=0;i<910;i++)
	{
		angle = 0.1 * (double)i;
		angle = angle*pi/180;
		SINETABLE[i] = (double)sin(angle);
	}
} 

double fastcosine(double x)
{
	return fastsine(x+90);
}

double fastsine(double x)
{
	int index;

	while(x<0)
	{
		x = x + 360;
	}
	while(x>=360)
	{
		x = x - 360;
	}
	index = (int)(x*10);
	if((index>=0 )&&(index<=900))
	{
		return SINETABLE[index];
	}
	else if((index>900)&&(index<=1800))
	{
		return SINETABLE[1800 - index];
	}
	else if((index>1800)&&(index<=2700))
	{
		return -1*SINETABLE[index-1800];
	}
	else if((index>2700)&&(index<=3600))
	{
		index = index - 1800;
		return -1*SINETABLE[1800-index];
	}
	else
	{
		return 0;
	}
}

double StrToDouble(TCHAR *Source, TCHAR **Stop)
{
  int index = 0;
  int StringLength        = 0;
  double Sum = 0;
  double Divisor = 10;

  StringLength = _tcslen(Source);

	while(Source[index] == ' ')
	{
		index ++;
	}

  while( (index < StringLength)    
          &&
          (
            (Source[index]>= '0') && (Source [index] <= '9')
          ) 
        )
  {
      Sum = (Sum*10) + (Source[ index ] - '0');
      index ++;
  }
  if(Source[index] == '.')
  {
    index ++;
    while( (index < StringLength)    
          &&
          (
            (Source[index]>= '0') && (Source [index] <= '9')
          ) 
        )
    {
      Sum = (Sum) + (double)(Source[ index ] - '0')/Divisor;
      index ++;Divisor = Divisor * 10;
   }
  }
  if(Stop != NULL)
    *Stop = &Source[index];

  return  Sum;
}