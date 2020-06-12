
## Suivi de projet
- L'avancée a été plutôt consistante, vous avez réussi à atteindre votre objectif, félicitations.

## Rendu
- README très propre.
- Git workflow OK.
- Norme OK.
- Pas de leaks impactants.
- Build fluide (Clone repo + F5 => Done).
- Format de fichier de sauvegarde propre.
- De nombreux composants implémentés.
- Editeur très bien réalisé.
- Un jeu qui somme toute est assez complexe, il prouve les capacités de votre moteur.
- Malgré la 3C qui mériterait encore du travail.
- Code dans l'ensemble plutôt clair et bien organisé.
- Le renderer est plutôt avancé. Peut-être que la démo contient un peu trop d'effets visuels, une scène de test spécial rendu aurait été un plus.
- Composants custom d'ImGui !
- Picking d'objet dans le viewport !
- Scripting !
- Animations !

## Build
- Build longue
- Mais qui marche du 1er coup :') (merci)

## Inspection du code
- La classe Light définie m_isActive qui est déjà un champs de ICompoent
	- Idem pour AudioListener
- Erreur dans `Mat4::operator*=`, vous faites une division au lieu d'une multiplication.
- `inline void SetPerspData(const CameraPerspective p)` inutile de mettre const si vous passez par copie. Où alors faites une const ref.
- En général, préférez retourner une const ref pour éviter des copie inutile, par exemple ici :
```c++
inline std::vector<std::string> FileSystem::GetExplodedCurrentPath()
// Devrait être
inline const std::vector<std::string>& FileSystem::GetExplodedCurrentPath()
```
- Fonction Ext::Explode à revoir :
	- Au lieu de `== ""` ou `!= ""`, utilisez la fonction `buff.empty()` `!buff.empty()`
	- Au lieu de `buff = "";`, utilisez `buff.clear();`
	- Condition `n == c` inutile (toujours true)
```c++
if (n != c)
	buff += n;
else if (n == c && buff != "")
	...
```
- Erreur dans la fonction `FileSystem::RenameContent`, `sizeof(newName)` retourne toujours la taille du pointeur (8)
	- https://godbolt.org/z/2pDNVR
- Logger::AddLog est déclarée comme `noexcept` alors qu'elle peut potentiellement lancer des exceptions (à cause du new)
- Beaucoup de dynamic_cast effectuée sans test du nullptr ensuite... Si vous êtes sûr du type, utilisez `static_cast`
- Dans `NavMeshBuilder::Build`, 
```c++
unsigned char* triareas{ new unsigned char[m_maxTris] };
if (!triareas) // Impossible, new déclenche une exception si l'allocation échoue
```
	- Vous devez utiliser
```c++
unsigned char* triareas{ new (std::nothrow) unsigned char[m_maxTris] };
if (!triareas) // OK
```
- La fonction `NavMeshBuilder::Build` mériterais d'être factorisée
	- 
```c++
bool comma{ false }; //Put comma before text
if (s > 0)
{
	if (comma) // Test inutile (toujours false)
```
- Erreur dans la fonction ClearForces :
```c++
void RigidBody::ClearForces()
{
	if (m_pxRigidBody) // Devrait être !m_pxRigidBody
		return;
	m_pxRigidBody->clearForce();
	m_pxRigidBody->clearTorque();
}
```
- Erreur dans Mesh::ChangeOneMaterial :
```c++
	if (newMaterial.find(".bmat"))	
	{ 
	// devrait être
	if (newMaterial.find(".bmat") != std::string::npos)	
	{
```
- Postprocess::OnInit(), factorisez pour la lisibilité, trop de `GetRoot()->GetEngine()->GetResourcesManager()`
- Idem dans `Material::LoadVariants` trop de `variants[index].var.get_type()`
- Undefined behavior dans Material::SaveMaterial
```c++
for (auto i = 0; i < variants.size();)
	save["Variant"][i++] = SaveVariants(i, resources);
```
	- Faites plutôt
```c++
for (size_t i = 0; i < variants.size(); i++)
	save["Variant"][i+1] = SaveVariants(i, resources);
```
- Leak ? Dans `1Node::CreateObjectScene` :
```c++
Core::Renderer::Mesh* mesh = new Core::Renderer::Mesh();

if (!isSkeletal)
	mesh = new Core::Renderer::Mesh();
```
- `Minion::OnEnterCollider` ??
```c++
if (object == m_Owen)
	object = m_Owen;
else if (object == m_Brioche)
	object = m_Brioche;
```
- Variable non initialisée :
```c++
physx::PxU32 filter; // Devrait être = 0 ici
filter |= static_cast<physx::PxU32>(Core::Physics::EFilterRaycast::GROUPE2);
```

## Editeur
- L'éditeur fonctionne plutôt bien ! Toute les fonctionnalités de bases sont présentes.
