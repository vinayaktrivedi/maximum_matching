#include<stdio.h>
#include<stdlib.h>

typedef struct path{
    int a;
    int b;
    struct path* next;
    struct path* prev;
}path;

int start_s;
int end_t;

path* make_bipartite_graph(int n,int** edge, int* matched,int** cycle,int* nodes, int step,int* parent,int* shrinked){
    //printf("no");
    if(step==0){
        for(int i=0;i<=n;i++){
            if(shrinked[i]>-1 || matched[i]==0){
                parent[i]=-1;
                nodes[i]=0;
            }
        }
    }
    
    int count=0;
    int check;
    if(step%2==0)
        check=0;
    else
        check=1;

    for(int i=0;i<=n;i++){
        if(nodes[i]==step){
            for(int j=0;j<=n;j++){
                if(edge[j][i]==check && nodes[j]==-1){
                    count++;
                    nodes[j]=step+1;
                    parent[j]=i;
                }
                else if(edge[j][i]==check && nodes[j]==step){
                    if(!(nodes[j]==0&&shrinked[j]==shrinked[i] &&shrinked[i]>-1)){
                        if(start_s==-1 && end_t==-1){
                           start_s = j;
                            end_t = i; 
                        }
                          
                    }
                    
                    
                }
            }
        }
    }
    
    if(count==0){
       
        path* g = (path*)malloc(sizeof(path));
        if(g==NULL)
            return NULL;

        if(start_s==-1 && end_t==-1){
            
            g->prev= NULL;
            g->next = NULL;
            g->a= -1;
            return g;
        }

        else{
            g->a=start_s;
            g->b = end_t;
            path* left=g;
            path* right=g;
            int i=start_s;
            int j=end_t;
            while(i!=j && !(nodes[i]==0 && nodes[j]==0) ){
                
                path* t_left = (path*)malloc(sizeof(path));
                path* t_right = (path*)malloc(sizeof(path));
                t_left->prev = NULL;
                t_right->next = NULL;
                t_left->next=left;
                t_right->prev=right;
                t_left->a = parent[i];
                t_left->b = i;
                t_right->a = j;
                t_right->b = parent[j];
                left->prev = t_left;
                right->next = t_right;
                i=parent[i];
                j=parent[j];
                left = t_left;
                right = t_right;
            }

            path* d = (path*)malloc(sizeof(path));

            if(d==NULL)
                return NULL;
            d->next = left;
            left->prev = d;
            d->prev = NULL;

            if(nodes[i]==0){
                if(i==j && shrinked[i]==-1){
                    d->b=1;
                    d->a=2;
                }
                else if(shrinked[i]==shrinked[j] && shrinked[i]!=-1){
                    d->b=0;
                    d->a=2;
                }
                else{
                    d->a=1;
                }
                return d;
            }

            else{

                path* c=d;
                d->a=parent[i];
                d->b=i;
                i=parent[i];
                while(nodes[i]!=0){
                    c = (path*)malloc(sizeof(path));
                    c->a=parent[i];
                    c->b=i;
                    c->prev=NULL;
                    c->next = d;
                    d->prev = c;
                    d=c;
                    i=parent[i];
                }

                path* final = (path*)malloc(sizeof(path));
                if(final==NULL)
                    return NULL;

                final->prev=NULL;
                final->next = c;
                c->prev = final;
                if(shrinked[i]==-1)
                    final->a = 3;
                else
                    final->a = 3;

                final->b = left->a;
                return final;
            }
        }
    }
    else{
        return make_bipartite_graph(n,edge,matched,cycle,nodes,step+1,parent,shrinked);
        //return NULL;
    }
}

path* find_aug(int n,int* matched, int** edge, int** cycle,int* shrinked,int count){
    
    int nodes[n+1];
    int parent[n+1];

    for(int i=0;i<=n;i++)
        nodes[i]=-1;

    for(int i=0;i<=n;i++)
        parent[i]=-2;
    start_s = -1;
    end_t = -1;
    path* walk = make_bipartite_graph(n,edge,matched,cycle,nodes,0,parent,shrinked);
    
    path* temp = walk;

    if(walk->a==-1)
        return NULL;

    else if(walk->a==1){

        walk = walk->next;
        path* ff = walk;

        path* t =walk;
        while(t->next!=NULL)
            t=t->next;

        path* begin = walk;
        path* fin = t;
        
        int f=begin->a;
        int q=fin->b;

        if(matched[f]==1){
            int i=0;
            while(cycle[f][i+1] !=-1){
                path* y=(path*)malloc(sizeof(path));
                y->next = begin;
                y->prev=NULL;
                begin->prev=y;
                y->a=cycle[f][i+1];
                y->b= cycle[f][i];
                begin = y;
                i++;
            }

        }

        if(matched[q]==1){
            int i=0;
            while(cycle[q][i+1] !=-1){
                path* y=(path*)malloc(sizeof(path));
                y->next = NULL;
                y->prev=fin;
                fin->next=y;
                y->a=cycle[q][i];
                y->b= cycle[q][i+1];
                fin = y;
                i++;
            }

        }

        return begin;
    }

    else if(walk->a==2){

        if(walk->b==1){
            while(temp->next!=NULL){
                temp = temp->next;
                shrinked[temp->a]=count;
                shrinked[temp->b]=count;

                int f = temp->a;
                int check=0;
                path* p;
                if(edge[temp->a][temp->b]==1){
                    check=1;
                    p=temp;
                }
                else
                    p = temp->prev;

                if(p==NULL)
                    return NULL;

                if(matched[f]==1){
                    if(check==0){
                        int i=0;
                    
                        while(p->prev!=NULL){
                            cycle[f][i] = p->b;
                            p = p->prev;
                            i++;
                        }
                        cycle[f][i] = p->next->a;
                    
                    }
                    else{
                        cycle[f][0] = p->a;
                        int i=1;
                        while(p!=NULL){
                            cycle[f][i] = p->b;
                            p = p->next;
                            i++;
                        }
                    }
                }
                else{
                    cycle[f][0] = f;
                }

            }
        }

        else{

            walk = walk->next;
            int s=shrinked[walk->a];

            while(temp->next!=NULL){
                temp = temp->next;
                int f=temp->b;
                if(shrinked[f]==-1){
                    shrinked[f] = s;
                    int check=0;
                    path* p;
                    if(edge[temp->a][temp->b]==1){
                        check=1;
                        p=temp;
                    }
                    else
                        p = temp;

                    if(p==NULL)
                        return NULL;

                    if(check==0){
                        int i=0;
                        while(p->next!=NULL){
                            p=p->next;
                            cycle[f][i] = p->a;
                            i++;
                        }
                        int u = p->b;
                        int j=0;
                        while(cycle[u][j]!=-1){
                            cycle[f][i] = cycle[u][j];
                            j++;
                            i++;
                        }
                    }
                    else{
                        cycle[f][0] = p->b;
                        int i=1;
                        
                        while(p->prev->prev !=NULL){
                            cycle[f][i] = p->a;
                            p = p->prev;
                            i++;
                        }

                        int u = p->a;
                        int j=0;
                        while(cycle[u][j]!=-1){
                            cycle[f][i] = cycle[u][j];
                            j++;
                            i++;
                        }
                    }
                }
            }
        }
        return find_aug(n,matched,edge,cycle,shrinked,count+1);
    }

    else if(walk->a==3){

        int v = walk->b;
        walk= walk->next;
        path* later = walk;
        if(walk==NULL)
            return NULL;

        int r = walk->a;
        int k=shrinked[r];
        //printf("entered %d\n",k);
        matched[v]=0;

        int i=0;
        int temp_path[n+1];
        for(int i=0;i<=n;i++)
        	temp_path[i]=-1;
        int special;

        while(walk->a!=v){
        	temp_path[i] = walk->a;
            if(i%2==0){
                edge[walk->a][walk->b]=1;
                edge[walk->b][walk->a]=1;
            }
            else{
                edge[walk->a][walk->b]=0;
                edge[walk->b][walk->a]=0;
            }
            walk = walk->next;
            if(walk==NULL)
                return NULL;
            i++;
        }
        temp_path[i] = v;

        i=0;
        while(cycle[r][i+1]!=-1){
            if(i%2==0){
                edge[cycle[r][i]][cycle[r][i+1]]=0;
                edge[cycle[r][i+1]][cycle[r][i]]=0;

            }
            else{
                edge[cycle[r][i]][cycle[r][i+1]]=1;
                edge[cycle[r][i+1]][cycle[r][i]]=1;
            }
            i++;
        }

        matched[cycle[r][i]] =1;

        int array[n+1];
        for(int i=0;i<=n;i++){
        	array[i]=-1;
        }

        int u=0;
        while(cycle[r][u]!=-1){
        	array[u]=cycle[r][u];
        	u++;
        }
        special = cycle[r][u-1];
        int fu = cycle[r][1];
        if(fu!=-1){
        	int t=0;
        	while(cycle[fu][t+2]!=-1)
        		t++;
        	while(cycle[fu][t]!=r){
        		array[u] = cycle[fu][t];
        		t--;
        		u++;
        	}
        	array[u] = r;
        }

        i=0;
        while(array[i+1]!=-1){
            //printf("yes");
        	shrinked[array[i]] = count;
        	if(array[i]==r){
        		for(int d=0;d<=n;d++)
        			cycle[r][d] = temp_path[d];
        	}
        	else{
        		if(edge[array[i]][array[i-1]]==1){
        			int d=i;
        			int j=0;
        			while(array[d]!=r){
        				cycle[array[i]][j] = array[d];
        				j++;
        				d--;
        			}
        			d=0;
        			while(j<=n){
        				cycle[array[i]][j] = cycle[r][d];
        				d++;
        				j++;
        			}
        		}
        		else{
        			int d=i;
        			int j=0;
        			while(array[d]!=r){
        				cycle[array[i]][j] = array[d];
        				j++;
        				d++;
        			}
        			d=0;
        			while(j<=n){
        				cycle[array[i]][j] = cycle[r][d];
        				d++;
        				j++;
        			}
        		}
        	}
        	i++;
        }
        
        temp = walk;
        while(temp!=NULL){
                int f=temp->a;
                shrinked[f]=count;
                int check=0;
                path* p;
                if(edge[temp->a][temp->b]==1){
                    check=1;
                    p=temp;
                }
                else
                    p = temp->prev;

                if(p==NULL)
                    return NULL;

                if(matched[f]==1){
                    if(check==0){
                        int i=0;
                        while(p->a!=v){
                            cycle[f][i] = p->b;
                            p = p->prev;
                            if(p==NULL)
                                return NULL;
                            i++;
                        }
                        cycle[f][i] = p->b;
                        cycle[f][i+1] = p->a;
                    }
                    else{
                        cycle[f][0] = p->a;
                        int i=1;
                        while(p!=NULL){
                            cycle[f][i] = p->b;
                            p = p->next;
                            i++;
                        }
                    }
                }
                else{
                	cycle[f][0] = f;
                }
                temp = temp->next;
        }
        return find_aug(n,matched,edge,cycle,shrinked,count);
    

    }
    return NULL;
}

int increase_matching(int n, int* matched,int** edge,int** cycle,int* shrinked){
    path* aug_path = find_aug(n,matched,edge,cycle,shrinked,0);
    if(aug_path == NULL)
        return -1;
    else{
            int i=0;
            while(aug_path != NULL){
                
                if(i==0)
                    matched[aug_path->a]=1;
                if(i%2==0){
                    edge[aug_path->a][aug_path->b]=1;
                    edge[aug_path->b][aug_path->a]=1;
                }
                else{
                    edge[aug_path->a][aug_path->b]=0;
                    edge[aug_path->b][aug_path->a]=0;
                }
                i++;
                if(aug_path->next==NULL)
                    matched[aug_path->b]=1;
                aug_path = aug_path->next;
            }
            
            return 1;
    }
}

int main(){

    int n,m;
    scanf("%d%d",&n,&m);
    int **edge;
    edge=(int**)malloc((n+1)*sizeof(int*));
    for(int i=0;i<=n;i++)
        *(edge+i) = (int*)malloc((n+1)*sizeof(int));
    for(int i=0;i<=n;i++){
        for(int j=0;j<=n;j++)
            edge[i][j]=-1;
    }

    int matched[n+1];
    for(int i=0;i<=n;i++)
        matched[i]=0;

    matched[0]=1;

    for(int i=0;i<m;i++){
        int a,b;
        scanf("%d%d",&a,&b);
        edge[a][b]=0;
        edge[b][a]=0;
    }

    
    int **cycle;
    cycle=(int**)malloc((n+1)*sizeof(int*));
    for(int i=0;i<=n;i++)
        *(cycle+i) = (int*)malloc((n+1)*sizeof(int));

    int shrinked[n+1];

    for(int j=0;j<n;j++){
        

        for(int i=0;i<=n;i++)
            shrinked[i]=-1;

        for(int i=0;i<=n;i++)
            for(int j=0;j<=n;j++)
                cycle[i][j]=-1;

        for(int i=0;i<=n;i++){
            if(matched[i]==0)
                cycle[i][0] = i;
        }


        int k= increase_matching(n,matched,edge,cycle,shrinked);
         if(k==-1)
             break;
    }


    int count=0;
    for(int i=0;i<=n;i++)
        if(matched[i]==1)
            count++;
    count--;
    printf("%d\n",count/2 );
    for(int i=1;i<=n;i++)
        for(int j=1;j<=i;j++)
            if(edge[i][j]==1)
                printf("%d %d\n",j,i);
    return 0;
   
}

