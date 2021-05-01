#include<bits/stdc++.h>
using namespace std;

double min_value,max_value,min_value2,max_value2;

struct point
{	
	///
	///  Rrepresents a Single Point in XY Coordinate
	///
	double x,y;
};
struct intervel
{
	///
	///  Rrepresents a Single Intervel in XY Coordinate
	///
	double bottom,top;	
};

struct rectangle
{	
	///
	///  Rrepresents a Single Rectangle in XY Coordinate
	///  Stores the bottom Left Point Coordinate and Top Right Coordinate of the Rectangle
	///
	double x_left,x_right,y_bottom,y_top;
	int id;
};

struct edge
{	
	///
	///  Rrepresents a Single edge in XY Coordinate
	///  Edgetype stores the type of edge we have with left = 0 , right = 1  , top = 2 , bottom = 3
	///  coordinate will store the single common point in edge across both end point and intervel will store the intervel of variable side
	///
	double coordinate; 
	int edgetype; 
	intervel * it;
	int id;
};

typedef struct bst bst; 
struct bst{

	///
	///  Rrepresents a Tree which is required for Contour Calculation in the Code
	///  Left and Right Stores the Left and Right Child Respectively
	///  x store the value and lru stores what type of node it is with left = 0 , right = 1 , undefined = 2 
	///
    bst * left; 
    bst * right; 
    double x; 
    char end;
    int lru; 
}; 

struct stripe
{	
	///
	///  Rrepresents a Single Stripe used in the Code
	///  X and Y stores the X and Y intervels of the Stripe
	///  Tree Stores the Tree of the stripe which is used in Contour Calculation
	///  x_measure stores the length of X Measure for that stripe
	///
	intervel* x;
	intervel* y;
	bst * tree;	
	double x_measure;
};


struct stripe_return_type // it will be returned from the STRIPES function
{	
	///
	///  Rrepresents a Return Type of Stripes Function used in the Code
	///  S is a set of Stripes here
	///  L and R are in accordance with the paper and contains the Y-Projections of all the edges in their Respective Sides
	///  P stores the Y projection of all endpoints of all line segment in vicinity and also cantains the upper and lower limits of the y boundary 
	///
	set<stripe *> S; 
	vector<pair<double,double> >  L, R;
	vector<double> p;
};


bst * createNode(double key, int lru , bst* l , bst* r){ 
	///
	///  Used To Create a Node of the tree used in the code
	///
    bst * node = (bst *)malloc(sizeof(bst)); 
    node -> left = l; 
    node -> right = r; 
    node -> x = key; 
    node -> lru = lru;

    return node; 
}   


bool compp(pair<double,double> p1,pair<double,double> p2){
	///
	///  Customized Comparing function used in Sorting the Arrays used in the code
	///
    if(p1.first  == p2.first){
        return (p1.second < p2.second);
    }
    return (p1.first < p2.first);
}

vector<pair<double,double> > unionn(vector<pair<double,double> > s1, vector<pair<double,double> > s2)
{
	///
	///  Used for calculation of Union of Arrays used in Stripes Function
	///

	vector<pair<double,double> > ans;
	set<pair<double,double> > st;
	for(int i=0;i<s1.size();i++)
		st.insert(s1[i]);
	for(int i=0;i<s2.size();i++)
		st.insert(s2[i]);
	auto it=st.begin();
	while(it!=st.end())
	{
		ans.push_back((*it));
		it++;
	}
	return ans;
}

vector<pair<double,double> > intersection(vector<pair<double,double> > s1, vector<pair<double,double> > s2)
{
	///
	///  Used for calculation of Intersection of Arrays used in Stripes Function
	///

	vector<pair<double,double> > ans;
	int i=0,j=0;
	while(i<s1.size() && j<s2.size())
	{
		if(s1[i].first==s2[j].first && s1[i].second==s2[j].second)
		{
			ans.push_back(s1[i]);
			i++;
			j++;
		}
		else if(s1[i].first==s2[j].first)
		{
			if(s1[i].second<s2[j].second)
				i++;
			else
				j++;
		}
		else if(s1[i].first<s2[j].first)
			i++;
		else
			j++;
	}
	return ans;
}

vector<pair<double,double> > diff(vector<pair<double,double> > s1, vector<pair<double,double> > s2)
{	
	///
	///  Used for calculation of difference of Arrays used in Stripes Function
	///

	if(is_sorted(s1.begin(), s1.end(), compp) == false){
        sort(s1.begin(),s1.end(),compp);
    }
	if(is_sorted(s2.begin(), s2.end(), compp) == false){
        sort(s2.begin(),s2.end(),compp);
    }
    
	// sort(s2.begin(),s2.end(),compp);

    vector<pair<double,double> > it=intersection(s1,s2);
    vector<pair<double,double> > ans;

    int i=0,j=0;

    while(i<s1.size() && j<it.size())
    {
        if(s1[i].first==it[j].first && s1[i].second==it[j].second)
		{
			i++;
			j++;
		}
		else
		{
			ans.push_back(s1[i]);
			i++;
		}

    }
    while(i<s1.size())
    {
		ans.push_back(s1[i]);  
		i++; 
    }
    return ans;
}


vector<double> find_union(vector<double> a, vector<double> b){
	///
	///  Used for calculation of Union of Arrays used in Stripes Function 
	///

    set<double> st;
	for(int i=0;i<a.size();i++)
		st.insert(a[i]);
	for(int i=0;i<b.size();i++)
		st.insert(b[i]);
	auto it=st.begin();
	vector<double> ans;
	while(it!=st.end())
	{
		ans.push_back((*it));
		it++;
	}
	return ans;
}

void printRectangles(vector<rectangle*> rectangle_array){
	///
	///  Helper Function Used to Print the Rectangle Corners 
	///

	for(int i=0;i<rectangle_array.size();i++){
		cout<<rectangle_array[i]->x_left<<" "<<rectangle_array[i]->y_bottom<<endl;
		cout<<rectangle_array[i]->x_right<<" "<<rectangle_array[i]->y_bottom<<endl;
		cout<<rectangle_array[i]->x_right<<" "<<rectangle_array[i]->y_top<<endl;
		cout<<rectangle_array[i]->x_left<<" "<<rectangle_array[i]->y_top<<endl;
	}
}


void printEdge(vector<edge*> edge_array){ 
	
	///
	///  Helper Function Used to Print the Specification of the Edges Obtained from Rectangle
	///
	for(int i=0;i<edge_array.size();i++){
		cout<<edge_array[i]->coordinate<<' '<<edge_array[i]->it->bottom<<' '<<edge_array[i]->it->top<<' '<<edge_array[i]->edgetype<<' '<<edge_array[i]->id<<' '<<endl;
	}
}

int compare( edge *  e1,  edge * e2){
	///
	///  Customized Compare Function to sort the edges so that they can be fed to Stripes Function
	///
    if(e1->coordinate  != e2->coordinate){
        return (e1->coordinate < e2->coordinate);
    }
    return (e1->edgetype < e2->edgetype);
}

int compare2( edge *  e1,  edge * e2){
	///
	///  Customized Compare Function to sort the edges so that they can be fed to countour_peices Function
	///
    if(e1->coordinate  == e2->coordinate){
		if(e1->it->bottom == e2->it->bottom){
			return e1->it->top < e2->it->top;
		}
		return  e1->it->bottom < e2->it->bottom;
    }
	return (e1->coordinate < e2->coordinate);
}

bool superset(intervel * a , intervel * b){
	///
	///  Helper function used to find out if one intervel is superset of another intervel
	///
	if(a->bottom <= b->bottom && a->top >= b->top) return true;
	return false;
}

set< stripe *> copyfunc(set <stripe *> S , vector<double> P , intervel * x_int){ 
	
	///
	///  Copy Function used in the paper 
	///  In first Part we are calculating the S' in accordence with the paper
	///  In second Part we are updating the X_measure and Tree as Specified by the paper
	///
	set<stripe * > Snew ; // Represent S'
	sort(P.begin(),P.end());
	for(int i=1;i<P.size();i++){
		stripe* temp  = new stripe();
		temp->x = x_int; 
		temp->y = new intervel();
		temp->y->bottom = P[i-1];
		temp->y->top = P[i];
		Snew.insert(temp);
	}

	for(set <stripe *> :: iterator it1 = Snew.begin() ; it1 != Snew.end() ; it1++){// Snew Represent S'
		for(set <stripe *> :: iterator it2 = S.begin() ; it2 != S.end() ; it2++){ // S Represent S
			if(superset((*it2)->y, (*it1)->y)){
                (*it1)->tree = (*it2)->tree; 
				(*it1)->x_measure = (*it2)->x_measure;
			}
		}
	}
	return Snew;
}


set<stripe*> concat(set<stripe*> a, set<stripe*> b, vector<double> P, intervel * x_int){

	///
	///  Concat Function used in the paper 
	///  In first Part we are calculating the S in accordence with the paper
	///  In second Part we are updating the X_measure and Tree as Specified by the paper
	///

    set<stripe*> res; 

    for(int i=1;i<P.size();i++){
		stripe* temp  = new stripe();
		temp->x = x_int; // not sure about it
		temp->y = new intervel();
		temp->y->bottom = P[i-1];
		temp->y->top = P[i];
		res.insert(temp);
	}


    for(set <stripe *> :: iterator s=  res.begin() ; s!=res.end() ; s++){
        stripe* alpha = NULL,  *beta = NULL; // alpha = s1 and beta = s2 after running the 2 loops below
        for(set <stripe *> :: iterator tempa = a.begin() ; tempa!= a.end() ; tempa++){// Taking s1 from S1 where tempa = s1 according to paper
            if((*tempa)->y->top  == (*s)->y->top && (*tempa)->y->bottom == (*s)->y->bottom){ 
				alpha = * tempa;
				break;
            }
        }
        for(set <stripe *> :: iterator tempb =  b.begin() ; tempb != b.end() ; tempb++){ // Taking s2 from S2 where tempb = s2 according to paper
            if((*tempb)->y->top == (*s)->y->top && (*tempb)->y->bottom == (*s)->y->bottom){
				beta = * tempb;
				break;
            }
        }

		if(alpha && beta ) { // Updating X measure and Tree in Accordance with the Paper
			(*s)->x_measure = alpha->x_measure + beta->x_measure; 

            if(alpha->tree == NULL && beta->tree == NULL){
                (*s)->tree  = NULL;
            } 
            else if(alpha->tree == NULL && beta->tree != NULL){
                (*s)->tree  =  beta->tree;
            }
            else if(alpha->tree != NULL && beta->tree == NULL){
                (*s)->tree  =  alpha->tree;
            }
            else{
                (*s)->tree  =  createNode(alpha->x->top , 2 , alpha->tree , beta->tree);
            }

		}
    }

    return res;
}

void blacken(set<stripe*> S,vector<pair<double,double> > J)
{
	///
	///  Blacken Function used in the paper 
	///  Updating X measure and Tree as specified in the Paper
	///
	auto it=S.begin();
	while(it!=S.end())
	{
		intervel* temp=(*it)->y;

		auto it2=J.begin();
		while(it2!=J.end())
		{
			if( (temp->bottom >= (*it2).first) && (temp->top <= (*it2).second) )
			{
				(*it)->x_measure=(*it)->x->top-(*it)->x->bottom;
                (*it)->tree = NULL; 
			}
			it2++;
		}		
		it++;
	}
}


void printbst(bst * root){
	///
	///  Helper Function Used for Printing the Tree Created while Calculating Contour  
	///
    if(root == NULL){
        return ; 
    }
    printbst(root -> left); 
    cout<<root->x<<" "<<root->lru<<"\n"; 
    printbst(root -> right); 

    return ; 
}


stripe_return_type * STRIPES(vector<edge*> edge_array, intervel * x_ext , intervel * y_ext){
	///
	///  Stripes Function used in the paper 
	///
	int num_edge = edge_array.size();

	if(num_edge == 1){ 

		///
		///  Case 1 when there is only one Edge
		///  Here we care calculating and updating all the Specification of Stripes return type as Specified by the Paper
		///

		edge* tempedge = edge_array[0];
		vector<pair<double,double> >  L, R;

		if (tempedge->edgetype == 0){ 
			// If the edge type is Left , append the intervel in Left side
			L.push_back(make_pair(tempedge->it->bottom , tempedge->it->top));
		}
		else{ 
			// If the edge type is Right , append the intervel in Right side
			R.push_back(make_pair(tempedge->it->bottom , tempedge->it->top));
		}
		vector<double> p{INT_MIN, tempedge->it->bottom , tempedge->it->top , INT_MAX};
		stripe* S  = new stripe();
		S->x = x_ext;
		S->y = new intervel();
		S->y->bottom = tempedge->it->bottom ;
		S->y->top = tempedge->it->top;

		if (tempedge->edgetype == 0){
			// If the edge type is Left 
			S->x_measure = x_ext->top - tempedge->coordinate;
            S->tree = createNode(tempedge->coordinate , 0 , NULL , NULL);
		}
		else{
			// If the edge type is Right
			S->x_measure = tempedge->coordinate - x_ext->bottom;	
            S->tree = createNode(tempedge->coordinate , 1, NULL , NULL);
		}
		stripe_return_type* stripe_return = new stripe_return_type();
		stripe_return->S.insert(S);
		stripe_return->L = L;
		stripe_return->R = R;
		stripe_return->p = p;

		return stripe_return ; // Return Stripes Return Type
	}
	else if(num_edge>1) {

		///
		///  Case 2 when there are Multiple Edge
		///  Here we care calculating and updating all the Specification of Stripes return type as Specified by the Paper
		///
		int median = num_edge/2;

		vector<edge*> v1 , v2;

		///
		///  Calculating V1 and V2 as specified by paper
		///

		for(int i=0;i<median;i++) v1.push_back(edge_array[i]);
		for(int i=median;i<num_edge;i++) v2.push_back(edge_array[i]);

		intervel * x1 = new intervel();
		intervel * x2 = new intervel();

		if(num_edge%2==0)
		{
			double  xm = ((edge_array[median-1])->coordinate + (edge_array[median])->coordinate) /2;
			x1->top = xm;
		}
		else
		{
			x1->top = (edge_array[median])->coordinate;
		}

		x1->bottom = x_ext->bottom;
		x2->top = x_ext->top;
		x2->bottom = x1->top;

		///
		///  Calculating Stripes Function Recursively on Left and Right side of Division As Specified by the paper
		///

		stripe_return_type * S1 = STRIPES(v1 , x1 , y_ext);
		stripe_return_type * S2 = STRIPES(v2 , x2 , y_ext);

		///
		///  Calculating L And R As Specified by the paper 
		///

		vector<pair<double,double> > LR , L , R;

		LR = intersection(S1->L , S2->R);
		vector<pair<double,double> > l1lr = diff(S1->L, LR), r2lr = diff(S2->R ,  LR);

		L = unionn(l1lr,S2->L);
		R = unionn(S1->R,r2lr);
		
		vector<double> P = find_union(S1->p , S2->p); // in implementation

		///
		///  Calling Copy function to create Sleft and Sright as Specified by the paper
		///

		set<stripe*> sleft = copyfunc(S1->S , P , x1);
		set<stripe*> sright = copyfunc(S2->S , P , x2);

		///
		///  Calling Blacken function to update Sleft and Sright as Specified by the paper
		///

	    blacken(sleft , r2lr );
		blacken(sright , l1lr );

		///
		///  Calling Concat function to Concatenate the Sleft and Sright and make Bigger Stripes as Specified by the paper
		///
		set<stripe*> Sf = concat(sleft , sright , P , x_ext);

		///
		///  Creating a stripe return type object and storing all the values in it and return it from the function
		///

		stripe_return_type * stripe_return = new stripe_return_type();
		stripe_return->S = Sf ;
		stripe_return->L = L;
		stripe_return->R = R;
		stripe_return->p = P;
		return stripe_return ;
	}
	return new stripe_return_type();
}

vector<pair<double,double> > intersection_contour(vector<pair<double,double> > s1, vector<pair<double,double> > s2)
{

	///
	///  Used for calculation of Intersection of Arrays used in contour_peices Function
	///
	if(s1.size()==0 && s2.size()==0)
		return vector<pair<double,double> >();

	if(is_sorted(s1.begin(), s1.end(), compp) == false){ 
        sort(s1.begin(),s1.end(),compp);
    }
	if(is_sorted(s2.begin(), s2.end(), compp) == false){
        sort(s2.begin(),s2.end(),compp);
    }

	vector<pair<double,double> > ans;

	int i=0,j=0;
	while( i<s1.size() && j<s2.size() )
	{
		if(s1[i].first>s2[j].second)
			j++;
		else if(s2[j].first>s1[i].second)
			i++;
		else
		{
			pair<double,double> temp;
            if(s1[i].first==s2[j].second)
            {
                j++;
                continue;
            }
            if(s1[i].second==s2[j].first)
            {
                i++;
                continue;
            }
			temp.first=max(s1[i].first,s2[j].first);
			temp.second=min(s1[i].second,s2[j].second);
			if(s1[i].second>s2[j].second)
				j++;
			else if(s1[i].second<s2[j].second)
				i++;
			else
			{
				i++;
				j++;	
			};
			ans.push_back(temp);
		}
	}
	return ans;
}

vector<pair<double,double> > diff_contour(vector<pair<double,double> > s1, vector<pair<double,double> > s2)
{	
	///
	///  Used for calculation of Difference of Arrays used in contour_peices Function
	///
	if(is_sorted(s1.begin(), s1.end(), compp) == false){
        sort(s1.begin(),s1.end(),compp);
    }
	if(is_sorted(s2.begin(), s2.end(), compp) == false){
        sort(s2.begin(),s2.end(),compp);
    }

    vector<pair<double,double> > it= intersection_contour(s1,s2);
    vector<pair<double,double> > ans;

    int i=0,j=0;

    while(i<s1.size() && j<it.size())
    {
        if(it[j].first >= s1[i].first && it[j].second <= s1[i].second )
        {
            if(s1[i].first!=it[j].first)
                ans.push_back({s1[i].first,it[j].first});
            if(s1[i].second!=it[j].second)
                ans.push_back({it[j].second,s1[i].second});
            i++;
            j++;
        }
        else
        {
            ans.push_back(s1[i]);
            i++;
        }
    }
    while(i<s1.size())
    {
        ans.push_back(s1[i]);
        i++;
    }
    return ans;
}

void union_from_stripe(vector<pair<double,double> > & ans , bst * root  , pair<double, double> & temp){
	
	///
	///  Used for calculation of Intervels from Tree that we created in Stripes Function
	///
    if(root == NULL) return;
    if(root->left) union_from_stripe(ans , root->left , temp);
    if(root->right) union_from_stripe(ans , root->right , temp);
    if(root->right == NULL && root->left == NULL){
        if(root->lru == 0){
            temp.first = root->x;
        }
        else if(root->lru == 1){
            temp.second = root->x;
            ans.push_back(temp);
        }
    }
}

vector<pair<point * ,point * >> countour_peices(edge * h , set<stripe *> S)
{
	///
	///  Used for calculation of Horizontal Contour Lines 
	///  Takes as input a sinle edge and all the stripes we created
	///  return all the horizontal intervels that are part of contour for that edge
	///
	vector<pair<point * ,point * >> ans;
	stripe * use;
	if(h->edgetype == 3){ // if the edgetype is bottom
		auto it = S.begin();
		while(it!=S.end()){
			if((*it)->y->top == h->coordinate){
				use = *it;
				break;
			}
			it++;
		}

	}
	else{ // if the edgetype is top
		auto it = S.begin();
		while(it!=S.end()){
			if((*it)->y->bottom == h->coordinate){
				use = *it;
				break;
			}
			it++;
		}
	}

	///
	///  Calculation of set of intervels J as Specified by the paper
	///

	vector<pair<double,double> > x_union_stripe ;
	vector<pair<double,double> > h_x_intervel {make_pair(h->it->bottom , h->it->top)};
	pair<double, double> temp ; 
	union_from_stripe(x_union_stripe , use->tree , temp);
	vector<pair<double,double>> intersec_temp = intersection_contour(h_x_intervel , x_union_stripe);
	vector<pair<double,double>> j = diff_contour(h_x_intervel , intersec_temp);


	for(int i= 0;i<j.size();i++){

		point * a = new point(), *b = new point();
		a->x = j[i].first;
		a->y = h->coordinate;
		b->x = j[i].second;
		b->y = h->coordinate;
		ans.push_back(make_pair(a, b));
	}
	return ans;
}

vector<pair<double,double> > contour_union(vector<pair<double,double> > s)
{
	///
	///  Used for calculation of Union of Arrays 
	///  Gives the Union of Intervels that helps us to merge and calculate the horizontal contour
	///
	if(s.size()==0)
		return vector<pair<double,double> >();
    // sort(s.begin(),s.end(),compp);
	if(is_sorted(s.begin(), s.end(), compp) == false){ 
        sort(s.begin(),s.end(),compp);
    }

    stack<pair<double,double> > st;
    st.push(s[0]);
    for (int i=1;i<s.size();i++)
    {
        pair<double,double> top = st.top();
        if (top.second < s[i].first)
            st.push(s[i]);
        else if (top.second < s[i].second)
        {
            top.second = s[i].second;
            st.pop();
            st.push(top);
        }
    }
    vector<pair<double,double> > ans;
    while (!st.empty())
    {
        pair<double,double> top = st.top();
        ans.push_back(top);
        st.pop();
    }
    return ans;
}


int main(){

	///
	///  Main function of the Code where all the data preprocessing happens along with the calculation of final value of Measure and  contour vertical lines and length of contour
	///
	int n;
	cin>>n;
	vector <rectangle*> rectangle_array; // stores all the rectangles given as input
	vector<edge*> edge_array; // contains the vertival lines of the rectangles
    vector<edge * > contour_array; // contais the horizontal lines of the rectanlges
	for(int i=0;i<n;i++) // taking inputs and storing them in respective arrays
	{
		double lbx, lby , rtx , rty;
		cin>> lbx >> lby>> rtx>> rty;
		// cin>>lbx>>rtx>>lby>>rty;

		if(i==0)
		{
			min_value2=lby;
			max_value2=rty;
		}
		else
		{
			min_value2=min(lby,min_value2);
			max_value2=max(rty,max_value2);
		}

		rectangle* temp = new rectangle();
		temp->x_left = lbx;
		temp->x_right = rtx;
		temp->y_bottom = lby;
		temp->y_top = rty;
		temp->id = i;
		rectangle_array.push_back(temp);

		// partially a part of Rectangle_dac of paper
		edge* righttempedge = new edge();
		righttempedge->edgetype = 1;
		righttempedge->coordinate = rtx;
		righttempedge->it = new intervel();
		righttempedge->it->bottom= lby;
		righttempedge->it->top= rty;
		righttempedge->id = i;

		edge_array.push_back(righttempedge);

		edge* lefttempedge = new edge();
		lefttempedge->edgetype = 0;
		lefttempedge->coordinate = lbx;
		lefttempedge->it = new intervel();
		lefttempedge->it->bottom= lby;
		lefttempedge->it->top= rty;
		lefttempedge->id = i;

		edge_array.push_back(lefttempedge);

        edge* toptempedge = new edge();
		toptempedge->edgetype = 2;
		toptempedge->coordinate = rty;
		toptempedge->it = new intervel();
		toptempedge->it->bottom= lbx;
		toptempedge->it->top= rtx;
		toptempedge->id = i;

		contour_array.push_back(toptempedge);


        edge* downtempedge = new edge();
		downtempedge->edgetype = 3;
		downtempedge->coordinate = lby;
		downtempedge->it = new intervel();
		downtempedge->it->bottom= lbx;
		downtempedge->it->top= rtx;
		downtempedge->id = i;

		contour_array.push_back(downtempedge);
	}
	sort(edge_array.begin() , edge_array.end() , compare ); // sort edges from left to right with left edge before right edge type
	
	min_value=edge_array[0]->coordinate;
	max_value=edge_array[edge_array.size()-1]->coordinate;

	intervel * x_ext = new intervel();
	x_ext->bottom = min_value;
	x_ext->top = max_value;

	intervel * y_ext = new intervel();
	y_ext->bottom = min_value2;
	y_ext->top = max_value2;

	// Calling the Stripes function 

	stripe_return_type * Su = STRIPES(edge_array, x_ext ,y_ext);
	// calculating the final values of measure which will be stored in ans
	double ans = 0;
	auto it=Su->S.begin();

	while(it!=Su->S.end())
	{	
		ans+= ((*it)->x_measure*((*it)->y->top - (*it)->y->bottom));
		it++;
	}

	cout<<"Measure = "<<ans<<endl;

	// Calculation of Contour Begins here

	vector<pair<double, double>> points; // all end points of all horizontal contour lines will be stored in this array
	vector<pair<point * ,point * >> contour ; // all end points of all contour lines will be stored in this array

	sort(contour_array.begin(),contour_array.end(), compare2);

	vector<pair<double, double>> bottom_intervel , top_intervel; // to hangle the case of same y but sumultanious lines which are top and bottom in differnt rectangle 
	// important to handle bottom line and top line case otherwise vertical lines calcuation part will fail
	double curr_y = INT_MIN;
    for(int i = 0;i<contour_array.size();i++){
        if(curr_y != contour_array[i]->coordinate){
			bottom_intervel = contour_union(bottom_intervel);
			top_intervel  = contour_union(top_intervel);

			for(int j = 0;j<bottom_intervel.size();j++){

				point * a = new point(), *b = new point();
				a->x = bottom_intervel[j].first;
				a->y = curr_y;
				b->x = bottom_intervel[j].second;
				b->y = curr_y;
				contour.push_back(make_pair(a, b));
				points.push_back(make_pair(a->x ,a->y ));
				points.push_back(make_pair(b->x ,b->y ));
				
			}

			for(int j = 0;j<top_intervel.size();j++){

				point * a = new point(), *b = new point();
				a->x = top_intervel[j].first;
				a->y = curr_y;
				b->x = top_intervel[j].second;
				b->y = curr_y;
				contour.push_back(make_pair(a, b));
				points.push_back(make_pair(a->x ,a->y ));
				points.push_back(make_pair(b->x ,b->y ));
				
			}

			curr_y =  contour_array[i]->coordinate;
			bottom_intervel.clear();
			top_intervel.clear();
		}
        vector<pair<point * ,point * >> temp = countour_peices(contour_array[i] , Su->S);
        for(int j = 0;j<temp.size();j++){

			if(contour_array[i]->edgetype == 3){
				bottom_intervel.push_back(make_pair(temp[j].first->x ,temp[j].second->x ));
			}
			else if(contour_array[i]->edgetype == 2){
				top_intervel.push_back(make_pair(temp[j].first->x ,temp[j].second->x ));
			}

		}
    }

	bottom_intervel = contour_union(bottom_intervel);
	top_intervel  = contour_union(top_intervel);
	// their y coordinate will be curr_y;

	for(int j = 0;j<bottom_intervel.size();j++){

		point * a = new point(), *b = new point();
		a->x = bottom_intervel[j].first;
		a->y = curr_y;
		b->x = bottom_intervel[j].second;
		b->y = curr_y;
		contour.push_back(make_pair(a, b));
		points.push_back(make_pair(a->x ,a->y ));
		points.push_back(make_pair(b->x ,b->y ));
		
	}

	for(int j = 0;j<top_intervel.size();j++){

		point * a = new point(), *b = new point();
		a->x = top_intervel[j].first;
		a->y = curr_y;
		b->x = top_intervel[j].second;
		b->y = curr_y;
		contour.push_back(make_pair(a, b));
		points.push_back(make_pair(a->x ,a->y ));
		points.push_back(make_pair(b->x ,b->y ));
		
	}

	// here points array stores all the end points of horizontal lines which will be used now in calculation of vertical lines 

    sort(points.begin(),points.end());
	int counting = 0;
	

	for(int i=1;i<points.size();i+=2){
		point * a = new point(), *b = new point();
		a->x = points[i].first;
		a->y = points[i].second;
		b->x = points[i-1].first;
		b->y = points[i-1].second;
		contour.push_back(make_pair(b, a));
	}	

	// Contour array stores all the end points of all contour lines and thus stores the contour answer

	cout<<"Number of Contour Lines = "<<contour.size()<<endl;
	double contour_value = 0;

	cout<<"Contour Line Segments:-"<<endl;

	// printing horizontal contour
	for(int i=0;i<contour.size();i++){
		point * a = contour[i].first , *b = contour[i].second;
		cout<<a->x<<' '<<a->y<<' ' <<b->x<<' ' <<b->y<<endl;

		contour_value += abs(b->x - a->x);
		contour_value += abs(b->y - a->y);
	}

	cout<<"Contour Length = "<<contour_value<<endl;

	return 0;
}