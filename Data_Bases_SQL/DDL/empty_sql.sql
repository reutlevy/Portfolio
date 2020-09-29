--Q1
SELECT FirstName,SalaryPerDay,Name,Description FROM (((ConstructorEmployee as CE INNER JOIN Employee as E ON (CE.EID=E.EID)) INNER JOIN ProjectConstructorEmployee AS PCE ON (E.EID = PCE.EID)) INNER JOIN Project AS P ON (P.PID = PCE.PID));

--Q2
SELECT Employee.*,Department.Name
FROM Employee
NATURAL JOIN OfficialEmployee NATURAL JOIN Department
UNION
SELECT Employee.*,Project.Name
FROM Employee
NATURAL JOIN (SELECT ProjectConstructorEmployee.EID,ProjectConstructorEmployee.PID ,max(ProjectConstructorEmployee.EndWorkingDate) FROM ProjectConstructorEmployee GROUP BY ProjectConstructorEmployee.EID)
NATURAL JOIN Project

--Q3
SELECT Neighborhood.Name, COUNT(*)FROM Neighborhood NATURAL JOIN Apartment WHERE (Apartment.NID = Neighborhood.NID) GROUP BY Neighborhood.Name HAVING count(*)>0 ORDER BY count(*) ASC;

--Q4
SELECT Resident.LastName,Apartment.Door,Apartment.Number,Apartment.NID,Apartment.SizeSquareMeter,Apartment.Type,Apartment.StreetName
 FROM Apartment LEFT JOIN Resident ON (Resident.Number=Apartment.Number AND Resident.StreetName=Apartment.StreetName AND Resident.Door=Apartment.Door);

--Q5
SELECT ParkingArea.AID, ParkingArea.Name,ParkingArea.NID, ParkingArea.PricePerHour,ParkingArea.MaxPricePerDay
FROM ParkingArea 
WHERE (MaxPricePerDay= (SELECT min(ParkingArea.MaxPricePerDay) FROM ParkingArea));

--Q6
SELECT CarParking.CID , CarParking.AID
FROM (CarParking
INNER JOIN ParkingArea ON CarParking.AID == ParkingArea.AID)
WHERE ParkingArea.MaxPricePerDay = ( SELECT min(ParkingArea.MaxPricePerDay) FROM ParkingArea )

--Q7
SELECT CAR.CID, Resident.RID
FROM ( ( ( ( Resident
INNER JOIN Apartment ON (Apartment.Number = Resident.Number AND Apartment.Door = Resident.Door AND Apartment.StreetName = Resident.StreetName))
INNER JOIN ParkingArea ON ParkingArea.NID = Apartment.NID) 
INNER JOIN CarParking ON (CarParking.AID = ParkingArea.AID AND CarParking.CID=CAR.CID))
INNER JOIN Car ON Car.ID = Resident.RID)
EXCEPT
SELECT CAR.CID, Resident.RID
FROM ( ( ( ( Resident 
INNER JOIN Apartment ON (Apartment.Number = Resident.Number AND Apartment.Door = Resident.Door AND Apartment.StreetName = Resident.StreetName))
INNER JOIN CarParking ON (CarParking.AID = ParkingArea.AID AND CarParking.CID=CAR.CID))
INNER JOIN Car ON Car.ID = Resident.RID)
INNER JOIN ParkingArea ON ParkingArea.NID != Apartment.NID)

--Q8
SELECT DISTINCT Resident.RID, Resident.FirstName, Resident.LastName FROM ( ( Resident INNER JOIN CAR ON Resident.RID = Car.ID) INNER JOIN CarParking ON CarParking.CID = CAR.CID) GROUP BY Resident.RID HAVING COUNT(CarParking.AID) IN (SELECT COUNT(ParkingArea.AID) FROM ParkingArea);

--Q9
CREATE VIEW r_ngbrhd AS
SELECT *
FROM Neighborhood
WHERE Neighborhood.Name like 'R%'