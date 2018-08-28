	    #include "Parser.h"


Parser::Parser(std::string languageLoc)
{
	this->language =languageLoc;
	this->lngInput = std::ifstream(this->language,std::ifstream::in);
}

Domain Parser::parseLanguage()
{
	Domain dom;
	std::string temp;
	char currentChar;
	int litterals = 0;
	while(this->lngInput.good()) {
		currentChar = this->lngInput.get();
		if (currentChar == '\n' || currentChar == ' ')
			continue;
		temp+=tolower(currentChar);
		if(temp.size() == 3 and temp == "def") {
			if (this->lngInput.peek() == ' ')
				this->lngInput.get();
			std::string type = "";
			getline(this->lngInput,type,'(');
			if (type == "noise") {
				this->parseNoise(this->lngInput,dom);
				temp = "";
			}
			else if (type == "observables") {
				this->parseObservables(this->lngInput);
				temp = "";
				litterals++;
				if(litterals == 2) {
					this->createLitterals(dom);
					litterals=0;
				}
			} else if (type == "primitives") {
				this->parsePrimitives(this->lngInput,dom);
				temp = "";
				litterals++;
				if(litterals == 2) {
					this->createLitterals(dom);
					litterals=0;
				}
			}
			else if (type=="actions")
			{
				break;
			}
			else if (type == "goals")
			{
				break;
			}
			else
				assert(false && " is not a definition that can be used in the domain.");

		} else if (temp.size() == 2 and temp == "/*") {
			std::string devNull;
			do {
				getline(this->lngInput,devNull,'*');
			} while (this->lngInput.peek() != '/');
			this->lngInput.get();
			temp = "";
		}
	}
	return dom;
}

void Parser::parseNoise(std::ifstream& lng, Domain& dom)
{
	char currentChar;
	std::string noiseType = "";
	do {
		currentChar = lng.get();
        //std::cout << noiseType << std::endl;
		if (currentChar == '\t' || currentChar == '\n' || currentChar == ' ')
			continue;
		if (currentChar == '(') {
			std::string val;
			getline(this->lngInput,val,')');
			std::string::size_type sz;
			float noiseRate = std::stof (val,&sz);
			if (noiseType == "obsrate") {
				this->oRate = noiseRate;
				noiseType = "";
			}
			else if (noiseType == "mutrate") {
				this->mRate = noiseRate;
				noiseType = "";
			}
			else if (noiseType == "addrate") {
				this->aRate = noiseRate;
				noiseType = "";
			}
			else
			{
				std::cout << noiseType << std::endl;
				assert(false && " n'est pas un bruit traitable.");
			}
		} else {
			noiseType +=currentChar;
		}

	} while (this->lngInput.good() && currentChar != ')');
	dom = Domain(this->oRate,this->mRate,this->aRate);
}
void Parser::parseObservables(std::ifstream& lng)
{
	std::string temp;
	char currentChar;
	do {
		currentChar = lng.get();

		if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == ')' )
			continue;
		lng.unget();
		getline(lng,temp);
		std::istringstream ss(temp);
		std::string tampon;
		getline(ss,tampon,',');

		assert(tampon.substr(tampon.length() -2) == "()");

		this->observables.insert(tampon.substr(0,tampon.length()-2));

	} while (lng.good() && currentChar != ')');
	assert(lng.good());
}

void Parser::parsePrimitives(std::ifstream& lng, Domain& dom)
{
	std::string temp;
	char currentChar;
	int openParenthesis = 1;
	bool actTime = false;
	Primitives pri;
	std::string::size_type sz;
	do {
		currentChar = lng.get();
		if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' )
			continue;
		else if (currentChar == '(') {
			openParenthesis++;
			if (openParenthesis == 2){
				pri.name = temp;
			}

		}
		else if(currentChar == ',' && openParenthesis == 2) {
			pri.nbVariables++;
		}
		else if(currentChar == '~' && openParenthesis == 2) {
			char type = 'u';
			std::string val1;
			std::string val2;
            //Soit u,n ou a
			type = lng.get();
            //La paranthese ouvrante qui suit peut etre saute
			lng.get();
            //On lit tout jusqu'a la paranthese fermante de l'actiontime
			getline(lng,val1,')');
			val2 = val1.substr(val1.find(',')+1);
			val1 = val1.substr(0,val1.find(','));
			float v1 = std::stof (val1,&sz);
			float v2 = std::stof (val2,&sz);
			pri.time = ActionTime(static_cast<Prob_law>(type),v1,v2);
		}
		else if(currentChar == ')') {
			openParenthesis--;
			if (openParenthesis == 1) {
				prim.push_back(pri);
				pri = Primitives();
			}
		}
		else
			temp+= currentChar;


	} while (lng.good() && openParenthesis > 0);
	assert(lng.good());

}

void Parser::parseActions(std::ifstream& lng, Domain& dom)
{

}

void Parser::parseGoals(std::ifstream& lng, Domain& dom)
{

}

void Parser::createLitterals(Domain& dom)
{
    std::string pos;
	std::vector<std::string> temp(this->observables.begin(), this->observables.end());
	for (std::vector<Primitives>::iterator it = prim.begin();it != prim.end();++it) {
        pos = (*it).name;
        pos += "(";
        //pos +=")";
        int nb = (*it).nbVariables;
        assert (nb <= this->observables.size());
        if (nb == 0)
            continue;
        do
        {
            std::vector<std::string>::iterator its = temp.begin();
            std::string literal = pos;
            literal+= *its;
            nb--;
            while(nb > 0) {
                ++its;
                literal += "," + *its;
                nb--;
            }
            literal += ")";
            dom.addLiteral(literal,true,false);

        }
        while(std::next_permutation(temp.begin(), temp.end()));

	}

}

std::string Parser::permuteLitterals(std::set<std::string> UnusedLitt, std::string currentLitt,int neededLit) {
	if (neededLit == 0)
		return currentLitt;
	for (std::set<std::string>::iterator it = UnusedLitt.begin();it != UnusedLitt.end();++it) {
		std::set<std::string> temp(UnusedLitt);
		temp.erase(*it);
		this->permuteLitterals(temp,currentLitt + "," + *it,neededLit-1);
	}


}



void Parser::setLanguage(std::string languageLoc)
{
	this->language =languageLoc;
	this->lngInput.close();
	this->lngInput = std::ifstream(this->language,std::ifstream::in);
	this->observables.clear();
	this->oRate = 1.0;
	this->aRate = 0.0;
	this->mRate = 0.0;

}

Parser::~Parser()
{
	this->lngInput.close();
}
